#include "WolfController.h"

#include "PauseScript.h"


void WolfController::Start()
{
	m_Rigidbody = &GetComponent<Rigidbody>();
	// the wolf is asleep by default
	// and therefore doesn't need to have its physics updated
	// as it does not move
	m_Rigidbody->SetSleeping(true);


	m_Transform = &GetComponent<Transform>();

	m_Collider = &GetComponent<BoxCollider>();

	// sleeping by default
	m_Animator = &GetComponent<Animator>();
	m_Animator->SetCurrentAnimation("sleep");

	Entity player = GetEntitiesWithTag("Player")[0];
	m_PlayerController = &GetNativeScript<PlayerController>(player);

	// everything the wolf can collide with
	m_GroundLayerMask = LayerManager::GetLayer("Ground");



	// set up the wolf's audio
	// each wolf needs its own sound as they all have different positions
	// and many wolves may by playing the same audio clip at the same time
	// but the audio system handles the sharing of the sound buffer so only
	// 1 sound buffer per unique asset will be loaded

	m_GrowlSound = "Growl" + std::to_string(GetEntityHandle());
	AudioSystem::LoadSound(m_GrowlSound, "assets/audio/growl.ogg");
	AudioSystem::SetLooping(m_GrowlSound, true);
	AudioSystem::SetMinimumDistance(m_GrowlSound, 96);
	AudioSystem::SetAttenuation(m_GrowlSound, 0.75f);


	m_FootstepsSound = "WolfFootsteps" + std::to_string(GetEntityHandle());
	AudioSystem::LoadSound(m_FootstepsSound, "assets/audio/wolfFootsteps.ogg");
	AudioSystem::SetLooping(m_FootstepsSound, true);


	m_AngrySound = "Angry" + std::to_string(GetEntityHandle());
	AudioSystem::LoadSound(m_AngrySound, "assets/audio/wolfChase.ogg");
	AudioSystem::SetLooping(m_AngrySound, true);
	AudioSystem::SetMinimumDistance(m_AngrySound, 96);
	AudioSystem::SetAttenuation(m_AngrySound, 0.75f);

	m_BiteSound = "Bite" + std::to_string(GetEntityHandle());
	AudioSystem::LoadSound(m_BiteSound, "assets/audio/bite.ogg");
	AudioSystem::SetRelativeToListener(m_BiteSound, true);


	AudioSystem::SetPosition(m_GrowlSound, GetCentre());
	AudioSystem::SetPosition(m_FootstepsSound, GetCentre());
	AudioSystem::SetPosition(m_AngrySound, GetCentre());
	
	// wolfs are asleep by default
	// so they should growl by default
	AudioSystem::PlaySound(m_GrowlSound);
}

void WolfController::Update(float ts)
{
	// dont update the wolf when the game is paused
	if (PauseScript::IsGamePaused()) return;

	// the distance to the player is used a lot
	// just calculate that at the start
	float dist = Math::SquareMagnitude(m_PlayerController->GetCentre() - GetCentre());


	switch (m_State)
	{
	case WolfState::Sleep:
		// the wolf doesnt do anything when its sleeping
		break;



	case WolfState::Wake:
		// check to see if the wolf has woken up
		if (!m_Animator->GetCurrentAnimation().IsPlaying())
		{
			// if the animation has stopped playing the wolf is awake
			m_State = WolfState::Alert;
			m_Animator->SetCurrentAnimation("alert");

			// the wolfs interest is reset when it wakes up
			m_Interest = m_InitialInterest;

			// let the wolf be affected by physics again
			m_Rigidbody->SetSleeping(false);
		}
		break;



	case WolfState::Alert:
		// check if the player is close enough to follow
		if (fabsf(dist) > m_MaxPlayerFollowDistance * m_MaxPlayerFollowDistance)
		{
			// the player is too far away
			// the wolf should start to lose interest
			m_Interest -= ts;
			if (m_Interest <= 0)
			{
				// the wolf has lost interest and is going to sleep
				m_State = WolfState::Sleep;
				m_Animator->SetCurrentAnimation("sleep");
				// play growling while wolf sleeps
				AudioSystem::StopSound(m_FootstepsSound);
				AudioSystem::PlaySound(m_GrowlSound);

				// the wolf is no longer going to be moving,
				// so we can disable its physics until it needs to move again
				m_Rigidbody->SetSleeping(true);
			}
			else
			{
				// let the wolf pace backwards and forwards
				// it moves at a constant speed
				
				// check if it runs against a wall
				m_AgainstWall = Physics::BoxCast({ m_Transform->GetPosition() + (m_FacingRight ? m_RightCastPoint : m_LeftCastPoint), m_CastSize }, m_GroundLayerMask).first;

				if (m_AgainstWall)
				{
					// the wolf has hit a wall!
					// it needs to get over it
					m_Animator->SetCurrentAnimation("idle");
					AudioSystem::StopSound(m_FootstepsSound);

					// it can do that by jumping or climbing
					if (fabsf(m_Rigidbody->GetVelocity().y) < 0.001f)
					{
						// the wolf is on the ground
						// test 1 tile above the wolfs current position
						bool tooTallToJump = Physics::BoxCast({ m_Transform->GetPosition() + (m_FacingRight ? m_RightCastPoint : m_LeftCastPoint) - sf::Vector2f{0, 32}, m_CastSize }, m_GroundLayerMask).first;

						if (tooTallToJump)
						{
							// if the wolf cant jump over the wall then change direction and pace back the way it came
							m_FacingRight = !m_FacingRight;
						}
						else
						{
							// the wall is short enough for the wolf to jump over
							m_Rigidbody->ChangeVelocity({ 0.0f, -250.0f });
						}
					}
				}
				else
				{
					// check first of all if we can actually move in the direction we want
					// sometimes wolves are blocked so they dont fall off critical edges they could not return back up to
					if ((m_DirectionBlock == 1 && m_FacingRight) || (m_DirectionBlock == -1 && !m_FacingRight))
					{
						m_Rigidbody->SetVelocity({ 0, m_Rigidbody->GetVelocity().y });
						m_Animator->SetCurrentAnimation("idle");
						AudioSystem::StopSound(m_FootstepsSound);
					}
					else
					{
						m_Rigidbody->SetVelocity({ m_PaceSpeed * (m_FacingRight ? 1 : -1), m_Rigidbody->GetVelocity().y });
						m_Animator->SetCurrentAnimation("walk");
						AudioSystem::PlaySound(m_FootstepsSound, false);
					}
				}

			}
		}
		else
		{
			// the player is close; the wolf should follow it
			m_State = WolfState::Follow;
			// the wolf becomes interested
			m_Interest = m_InitialInterest;
			AudioSystem::PlaySound(m_AngrySound);

			
		}
		break;



	case WolfState::Follow:
		// the wolf is following the player
		// first check if the player has moved out of range
		if (fabsf(dist) > m_MaxPlayerFollowDistance * m_MaxPlayerFollowDistance)
		{
			// the player is now too far away, so the wolf should go on alert instead
			m_State = WolfState::Alert;
			m_Animator->SetCurrentAnimation("alert");

			AudioSystem::StopSound(m_AngrySound);
			AudioSystem::StopSound(m_FootstepsSound);

			
		}
		else
		{
			// the player is close; the wolf should move towards it
			sf::Vector2f diff = m_PlayerController->GetCentre() - GetCentre();
			if (fabsf(diff.y) < m_VerticalThresholdForDirectionChange)
				m_FacingRight = diff.x > 0;

			// check if the wolf is against the wall
			m_AgainstWall = Physics::BoxCast({ m_Transform->GetPosition() + (m_FacingRight ? m_RightCastPoint : m_LeftCastPoint), m_CastSize }, m_GroundLayerMask).first;

			// check if the player is far enough away to follow
			if (fabsf(diff.x) > m_MinPlayerFollowDistance)
			{
				if (m_AgainstWall)
				{
					// the wolf has hit a wall!
					// it needs to get over it
					m_Animator->SetCurrentAnimation("idle");
					AudioSystem::StopSound(m_FootstepsSound);

					// it can do that by jumping or climbing
					if (fabsf(m_Rigidbody->GetVelocity().y) < 0.001f)
					{
						// the wolf is on the ground
						// test 1 tile above the wolfs current position
						bool tooTallToJump = Physics::BoxCast({ m_Transform->GetPosition() + (m_FacingRight ? m_RightCastPoint : m_LeftCastPoint) - sf::Vector2f{0, 32}, m_CastSize }, m_GroundLayerMask).first;

						// make sure we dont start climbing a 2-block high wall with a roof above it, cause this will look glitchy
						bool tooShortToClimb = Physics::BoxCast({ m_Transform->GetPosition() + (m_FacingRight ? m_RightCanClimbCastPoint : m_LeftCanClimbCastPoint), m_ClimbRoofCastSize }, m_GroundLayerMask).first;
						
						// is the wall too high?
						if (tooTallToJump)
						{
							// a 2-block high wall with a roof above it is too short to climb
							if (!tooShortToClimb)
							{
								// the wall is too high for the wolf to jump over
								// it should climb it instead
								m_State = WolfState::Climb;
								m_Animator->SetCurrentAnimation("climb");
								
								StartClimb();
							}
						}
						else
						{
							// the wall is short enough for the wolf to jump over
							m_Rigidbody->ChangeVelocity({ 0.0f, -250.0f });
						}

					}
				}
				else
				{
					// we are waiting for other wolves to pass so that they dont all bunch together
					if (m_Stalling)
					{
						// just be idle for a bit
						m_Rigidbody->SetVelocity({ 0, m_Rigidbody->GetVelocity().y });
						m_Animator->SetCurrentAnimation("idle");
						AudioSystem::StopSound(m_FootstepsSound);
					}
					else
					{
						// ts is not a factor in the velocity
						// as it is not a change of velocity over time; it is being set to a constant value
						// then the delta time will be taken into account when the velocity is applied to the position in the physics system

						// check first of all if we can actually move in the direction we want
						// sometimes wolves are blocked so they dont fall off critical edges they could not return back up to
						if ((m_DirectionBlock == 1 && m_FacingRight) || (m_DirectionBlock == -1 && !m_FacingRight))
						{
							m_Rigidbody->SetVelocity({ 0, m_Rigidbody->GetVelocity().y });
							m_Animator->SetCurrentAnimation("idle");
							AudioSystem::StopSound(m_FootstepsSound);
						}
						else
						{
							// move in the direction its facing
							m_Rigidbody->SetVelocity({ m_MoveSpeed * (m_FacingRight ? 1 : -1), m_Rigidbody->GetVelocity().y });
						}

						// set the animation of the wolf according to how fast its moving
						if (fabsf(m_Rigidbody->GetVelocity().x) > 100)
						{
							m_Animator->SetCurrentAnimation("run");
							AudioSystem::PlaySound(m_FootstepsSound, false);
						}
						else if (fabsf(m_Rigidbody->GetVelocity().x) > 50)
						{
							m_Animator->SetCurrentAnimation("walk");
							AudioSystem::PlaySound(m_FootstepsSound, false);
						}
					}
				}
			}
			// if the player isnt already dead the wolf can attack
			else if (!m_PlayerController->IsDead())
			{
				// the player is too close to follow
				// check if they are close enough to attack
				if (fabsf(dist) < m_AttackDist * m_AttackDist)
				{// the player is really close; within biting range!
				// make sure we havent JUST attacked the player
					if (m_AttackCooldown <= 0)
					{
						// attack time
						m_State = WolfState::Attack;
						// play the attack animation
						m_Animator->SetCurrentAnimation("bite");
						m_Animator->Reset();

						// play the bite sound
						AudioSystem::StopSound(m_FootstepsSound);
						AudioSystem::PlaySound(m_BiteSound);


						// apply the attack cooldown
						m_AttackCooldown = m_InitialAttackCooldown;

						// hurt the player to register the bite
						m_PlayerController->Hurt(diff.x > 0);
					}
					else
					{
						// idle until something changes
						m_Animator->SetCurrentAnimation("idle");
						AudioSystem::StopSound(m_FootstepsSound);
					}
				}
				else
				{
					// idle until something changes
					m_Animator->SetCurrentAnimation("idle");
					AudioSystem::StopSound(m_FootstepsSound);
				}
				// stop moving for now
				m_Rigidbody->SetVelocity({ 0, m_Rigidbody->GetVelocity().y });
			}
			else
			{
				// idle until something changes
				m_Animator->SetCurrentAnimation("idle");
				AudioSystem::StopSound(m_FootstepsSound);
			}
			// make the attack cool down
			m_AttackCooldown -= ts;
		}
		break;



	case WolfState::Attack:
		// the wolf should be attacking the player
		if (!m_Animator->GetCurrentAnimation().IsPlaying())
		{
			// if the animation has stopped playing the wolf has finished biting
			// we go back to chasing the player
			m_State = WolfState::Follow;
		}
		break;


	case WolfState::Climb:

		// the wolf will always climb up
		// it can just jump off the top to get down
		m_Rigidbody->SetVelocity({ m_Rigidbody->GetVelocity().x, -m_ClimbSpeed });


		// decide when to stop climbing
		// cast in front of the climbing wolf
		// if there is no collision then we can do back to walking
		bool stopClimb = !Physics::BoxCast({ m_Transform->GetPosition() + (m_FacingRight ? m_ClimbRightCastPoint : m_ClimbLeftCastPoint), m_ClimbCastSize }, m_GroundLayerMask).first;
		if (stopClimb)
		{
			// stop climbing and return to follow state
			EndClimb();
			m_State = WolfState::Follow;
		}
		else
		{
			// we do not want a wolf to get stuck against the ceiling, so we need to check that it hasn't hit the roof
			// do another cast to see if theres a roof above us
			bool hitRoof = Physics::BoxCast({ m_Transform->GetPosition() + m_ClimbRoofCastPoint, m_ClimbRoofCastSize }, m_GroundLayerMask).first;
			if (hitRoof)
			{
				EndClimbFromRoof();

				m_State = WolfState::Follow;
			}
		}


		break;
	}


	// move the sounds to reflect the wolfs current position
	AudioSystem::SetPosition(m_GrowlSound, GetCentre());
	AudioSystem::SetPosition(m_FootstepsSound, GetCentre());
	AudioSystem::SetPosition(m_AngrySound, GetCentre());

	// if the wolf is facing left all of its animations must be flipped
	m_Animator->SetFlipped(!m_FacingRight);
}

void WolfController::OnTriggerEnter(const Collision& collision)
{
	// check if the wolf collided with a blocker
	if (GetEntityTag(collision.Other) == "WolfBlocker")
	{
		// we only want to compare horizontal diffs, which is why we dont use collision.CollisionDirection
		// as if it is Up or Down then that would be problematic
		if (Math::Centroid(collision.OtherGlobalBounds).x > GetCentre().x)
		{
			// we cannot move to the right at the moment
			m_DirectionBlock = 1;
		}
		else
		{
			// we cannot move left
			m_DirectionBlock = -1;
		}
	}
}

void WolfController::OnTriggerExit(const std::pair<Entity, ColliderID>& other)
{
	// the wolf is no longer being blocked
	if (GetEntityTag(other.first) == "WolfBlocker")
	{
		m_DirectionBlock = 0;
	}
}


void WolfController::Wake()
{
	// wake up the wolf
	if (m_State == WolfState::Sleep)
	{
		// change its state to wake
		m_State = WolfState::Wake;
		m_Animator->SetCurrentAnimation("wake");
		// the wolf does not growl when woken
		AudioSystem::StopSound(m_GrowlSound);
	}
}


void WolfController::StartClimb()
{
	// change the wolfs hitbox
	m_Collider->Reset(m_ClimbHitbox);

	// position the wolf against the wall
	if (m_FacingRight)
	{
		// move it one tile to the right and one tile up
		m_Transform->Translate({ 32, -32 });
	}
	else
	{
		// if its facing left we only need to move it 1 tile up
		m_Transform->Translate({ 0, -32 });
	}
}

void WolfController::EndClimb()
{
	// change the wolfs hitbox
	m_Collider->Reset(m_StandHitbox);

	// position the wolf on top of the wall
	// the position only needs adjusted for facing left
	if (!m_FacingRight)
	{
		// the wolf needs moved 1 tile left
		m_Transform->Translate({ -32, 0 });
	}
}

void WolfController::EndClimbFromRoof()
{
	// go back to the standing hitbox
	m_Collider->Reset(m_StandHitbox);

	// this function is called when the wolf is stuck at the roof
	// so we need to help it get out of the roof
	if (m_FacingRight)
	{
		m_Transform->Translate({ -32, 0 });

		// the wolf should then face away from the wall it just got stuck in
		m_FacingRight = false;
	}
	else
	{
		m_FacingRight = true;
	}

}

sf::Vector2f WolfController::GetCentre()
{
	// find out the centre position of the wolf
	if (m_State == WolfState::Climb)
		return m_Transform->GetPosition() + sf::Vector2f{ 16, 32 };
	else
		return m_Transform->GetPosition() + sf::Vector2f{ 32, 16 };
}
