#include "WolfController.h"


void WolfController::Start()
{
	m_Rigidbody = &GetComponent<Rigidbody>();
	m_Transform = &GetComponent<Transform>();

	m_Collider = &GetComponent<BoxCollider>();

	m_Animator = &GetComponent<Animator>();
	m_Animator->SetCurrentAnimation("sleep");

	Entity player = GetEntitiesWithTag("Player")[0];
	m_PlayerController = &GetNativeScript<PlayerController>(player);

	m_GroundLayerMask = LayerManager::GetLayer("Ground");
}

void WolfController::Update(Timestep ts)
{
	float dist = Math::SquareMagnitude(m_PlayerController->GetCentre() - m_Transform->Position - sf::Vector2f{ 32, 16 });

	if (Input::IsKeyPressed(sf::Keyboard::R)) Wake();


	switch (m_State)
	{
	case WolfState::Sleep:
		// the wolf doesnt do anything when its sleeping
		break;



	case WolfState::Wake:
		// check to see if the wolf has woken up
		if (!m_Animator->GetCurrentAnimation().Playing)
		{
			// if the animation has stopped playing the wolf is awake
			m_State = WolfState::Alert;
			m_Animator->SetCurrentAnimation("alert");

			// the wolfs interest is reset when it wakes up
			m_Interest = m_InitialInterest;
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
			}
		}
		else
		{
			// the player is close; the wolf should follow it
			m_State = WolfState::Follow;
			// the wolf becomes interested
			m_Interest = m_InitialInterest;
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
			m_Rigidbody->Velocity.x = 0;

		}
		else
		{
			// the player is close; the wolf should move towards it
			float diff = m_PlayerController->GetCentre().x - m_Transform->Position.x - 32;
			m_FacingRight = diff > 0;

			m_AgainstWall = Physics::BoxCast({ m_Transform->Position + (m_FacingRight ? m_RightCastPoint : m_LeftCastPoint), m_CastSize }, m_GroundLayerMask).first;

			m_Rigidbody->Velocity.x = Math::Lerp(m_Rigidbody->Velocity.x, 0.0f, m_Friction * ts);


			if (fabsf(diff) > m_MinPlayerFollowDistance)
			{
				if (m_AgainstWall)
				{
					// the wolf has hit a wall!
					// it needs to get over it
					m_Animator->SetCurrentAnimation("idle");

					// it can do that by jumping or climbing
					if (fabsf(m_Rigidbody->Velocity.y) < 0.001f)
					{
						// the wolf is on the ground
						// test 1 tile above the wolfs current position
						bool shouldClimb = Physics::BoxCast({ m_Transform->Position + (m_FacingRight ? m_RightCastPoint : m_LeftCastPoint) - sf::Vector2f{0, 32}, m_CastSize }, m_GroundLayerMask).first;
						if (shouldClimb)
						{
							// the wall is too high for the wolf to jump over
							// it should climb it instead
							m_State = WolfState::Climb;
							m_Animator->SetCurrentAnimation("climb");
							StartClimb();
						}
						else
						{
							// the wall is short enough for the wolf to jump over
							m_Rigidbody->Velocity.y = -250.0f;
						}

					}
				}
				else
				{
					// ts is not a factor in the velocity
					// as it is not a change of velocity over time; it is being set to a constant value
					// then the delta time will be taken into account when the velocity is applied in the physics system
					m_Rigidbody->Velocity.x = m_MoveSpeed * (m_FacingRight ? 1 : -1);

					// set the animation of the wolf according to how fast its moving
					if (fabsf(m_Rigidbody->Velocity.x) > 100)
					{
						m_Animator->SetCurrentAnimation("run");
					}
					else if (fabsf(m_Rigidbody->Velocity.x) > 50)
					{
						m_Animator->SetCurrentAnimation("walk");
					}
				}
			}
			else
			{
				// the player is too close to follow
				// check if they are close enough to attack
				if (fabsf(dist) < m_AttackDist * m_AttackDist)
				{// the player is really close; within biting range!
				// make sure we havent JUST attacked the player
					if (m_AttackCooldown <= 0)
					{
						m_State = WolfState::Attack;
						m_Animator->SetCurrentAnimation("bite");
						m_Animator->Reset();

						m_AttackCooldown = m_InitialAttackCooldown;

						m_PlayerController->Hurt(diff > 0);
					}
					else
					{
						m_Animator->SetCurrentAnimation("idle");
					}
				}
				else
				{
					m_Animator->SetCurrentAnimation("idle");
				}
				m_Rigidbody->Velocity.x = 0;
			}


			// make the attack cool down
			m_AttackCooldown -= ts;
		}
		break;



	case WolfState::Attack:
		// the wolf should be attacking the player
		if (!m_Animator->GetCurrentAnimation().Playing)
		{
			// if the animation has stopped playing the wolf has finished biting
			// we go back to chasing the player
			m_State = WolfState::Follow;
		}
		break;


	case WolfState::Climb:

		// the wolf will always climb up
		// it can just jump off the top to get down
		m_Rigidbody->Velocity.y = -m_ClimbSpeed;


		// decide when to stop climbing
		// cast in front of the climbing wolf
		// if there is no collision then we can do back to walking
		bool stopClimb = !Physics::BoxCast({ m_Transform->Position + (m_FacingRight ? m_ClimbRightCastPoint : m_ClimbLeftCastPoint), m_ClimbCastSize }, m_GroundLayerMask).first;
		if (stopClimb)
		{
			// stop climbing and return to follow state
			EndClimb();
			m_State = WolfState::Follow;
		}

		break;
	}



	m_Animator->Flip = !m_FacingRight;
}


void WolfController::Wake()
{
	if (m_State == WolfState::Sleep)
	{
		m_State = WolfState::Wake;
		m_Animator->SetCurrentAnimation("wake");
	}
}


void WolfController::StartClimb()
{
	m_Collider->Reset(m_ClimbHitbox);

	// position the wolf against the wall
	if (m_FacingRight)
	{
		// move it one tile to the right and one tile up
		m_Transform->Position += sf::Vector2f{ 32, -32 };
	}
	else
	{
		// if its facing left we only need to move it 1 tile up
		m_Transform->Position.y -= 32;
	}
}

void WolfController::EndClimb()
{
	m_Collider->Reset(m_StandHitbox);

	// position the wolf on top of the wall
	// the position only needs adjusted for facing left
	if (!m_FacingRight)
	{
		// the wolf needs moved 1 tile left
		m_Transform->Position.x -= 32;
	}
}