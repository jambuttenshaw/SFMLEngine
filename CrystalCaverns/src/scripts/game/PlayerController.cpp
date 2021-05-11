#include "PlayerController.h"

#include "PauseScript.h"


void PlayerController::Start()
{
	// get the components on the player that we need to manipulate
	m_Transform = &GetComponent<Transform>();
	m_Rigidbody = &GetComponent<Rigidbody>();
	m_Animator = &GetComponent<Animator>();
	m_Collider = &GetComponent<BoxCollider>();

	m_StatsController = &GetNativeScript<PlayerStatsController>();

	m_Light = &GetComponent<Transform>(GetEntitiesWithTag("PlayerLight")[0]);
	m_LightAnimator = &GetNativeScript<PlayerLightAnimator>();

	m_CameraController = &GetNativeScript<CameraController>(GetEntitiesWithTag("MainCamera")[0]);

	// the ground layer mask is things that the player can collide with
	m_GroundLayerMask = LayerManager::GetLayer("Ground") | LayerManager::GetLayer("JumpThrough");

	// the cast points depend on whether the player is crouching or standing
	// and define where the player looks to see if there is any level geometry
	// so it knows how it can move
	m_LeftCastPoint = { 7, 16 };
	m_RightCastPoint = { 25, 16 };
	m_BottomCastPoint = { 8, 64 };

	m_HorizontalCastSize = { 17, 0.5f };
	m_VerticalCastSize = { 0.5f, 48 };



	// load in the players sounds
	// these are all relative to listener, as the player is the listener
	AudioSystem::LoadSound("ladderCreak", "assets/audio/ladderCreak.ogg", 30);
	AudioSystem::SetLooping("ladderCreak", true);
	AudioSystem::SetRelativeToListener("ladderCreak", true);
	
	AudioSystem::LoadSound("footsteps", "assets/audio/footsteps.ogg");
	AudioSystem::SetLooping("footsteps", true);
	AudioSystem::SetRelativeToListener("footsteps", true);

	AudioSystem::LoadSound("crawlingFootsteps", "assets/audio/crawlingSteps.ogg", 45);
	AudioSystem::SetLooping("crawlingFootsteps", true);
	AudioSystem::SetRelativeToListener("crawlingFootsteps", true);

	AudioSystem::LoadSound("jump", "assets/audio/jump.ogg", 50);
	AudioSystem::SetRelativeToListener("jump", true);
	AudioSystem::LoadSound("jumpImpact", "assets/audio/jumpImpact.ogg", 30);
	AudioSystem::SetRelativeToListener("jumpImpact", true);
}

void PlayerController::Update(float ts)
{

	// if the game is paused: the player does nothing
	if (PauseScript::IsGamePaused()) return;


	// all sounds are heard from the perspective of the player
	AudioSystem::SetListenerPosition(GetCentre());


	// friction should ALWAYS be applied to the player
	sf::Vector2f oldVel{ m_Rigidbody->GetVelocity() };
	m_Rigidbody->SetVelocity({ Math::Lerp(oldVel.x, 0.0f, m_Friction * ts), oldVel.y });


	// all user input response needs to be below this
	if (m_State == PlayerState::Dead) return;

	// only check for ground if were not on a jump through platform
	if (!(m_OnJumpThrough && m_CanLandOnPlatform))
		m_OnGround = Physics::BoxCast({ m_Transform->GetPosition() + m_BottomCastPoint, m_HorizontalCastSize }, m_GroundLayerMask).first;

	// perform a box cast to check if the players against the wall
	if (m_FacingRight)
		m_AgainstWall = Physics::BoxCast({ m_Transform->GetPosition() + m_RightCastPoint, m_VerticalCastSize }, m_GroundLayerMask).first;
	else
		m_AgainstWall = Physics::BoxCast({ m_Transform->GetPosition() + m_LeftCastPoint, m_VerticalCastSize }, m_GroundLayerMask).first;


	// if we can land on a jump through platform
	if (m_CanLandOnPlatform)
	{
		// if we are pressing S then we should drop through them
		if (Input::IsKeyDown(sf::Keyboard::S))
		{
			m_CanLandOnPlatform = false;

			// if the player is climbing a ladder, then different behaviour is produced when pressing S
			if (m_State != PlayerState::Climb)
			{// the player should move to jump state after falling down through a platform
				if (m_State == PlayerState::Crawl)
					// if the player is crawling they need to stop before dropping down
					EndCrawl();

				m_Animator->SetCurrentAnimation("jump");
				m_State = PlayerState::Jump;
			}
		}
	}
	else
	{
		// we are no longer on a jump through and S is no longer held down
		// we are able to land on another jump through
		if (!m_OnJumpThrough && !Input::IsKeyDown(sf::Keyboard::S))
			m_CanLandOnPlatform = true;
	}



	switch (m_State)
	{
	case PlayerState::Grounded:

		// check if the player has left the ground
		if (!(m_OnGround || m_OnJumpThrough))
		{
			// stop the footsteps
			AudioSystem::StopSound("footsteps");
			if (m_LadderContacts)
			{
				// the player has left the ground but is now on a ladder
				// so they should begin climbing the ladder
				m_Animator->SetCurrentAnimation("climb");
				m_State = PlayerState::Climb;
				AudioSystem::PlaySound("ladderCreak");
			}
			else
			{
				// if theyre not on the ground and not on a ladder they must be jumping
				m_Animator->SetCurrentAnimation("jump");
				m_State = PlayerState::Jump;
			}

			// the player should do no more 'grounded' activities
			break;
		}


		// the player can begin crawling 
		if (Input::IsKeyDown(sf::Keyboard::LControl))
		{
			StartCrawl();
			// check if the player actually began crawling
			// it may not have actually started crawling if there was not space
			// to crawl in
			if (m_State == PlayerState::Crawl)
			{
				// dont do any other grounded activities if were now crawling
				AudioSystem::StopSound("footsteps");
				m_Animator->SetCurrentAnimation("idleCrawl");
				break;
			}
		}


		// the player can move while on the ground
		Move(ts);

		// set the run animation when the player is moving fast enough
		if (fabsf(m_Rigidbody->GetVelocity().x) > 100.0f)
		{
			// play the footsteps sound
			m_Animator->SetCurrentAnimation("run");
			AudioSystem::PlaySound("footsteps", false);
		}
		else
		{
			// set the idle animation and stop the footsteps sound
			m_Animator->SetCurrentAnimation("idle");
			AudioSystem::StopSound("footsteps");
		}
		// make sure to jump after setting the run animation
		// if the player does jump, then we want to change the animation

		// the player can jump while on the ground
		if (Input::IsKeyDown(sf::Keyboard::W))
		{
			// the player is attempting to jump

			// check if theyre in contact with a ladder first
			if (m_LadderContacts)
			{
				// transition to climbing state
				AudioSystem::StopSound("footsteps");

				m_State = PlayerState::Climb;
				m_Animator->SetCurrentAnimation("climb");
				AudioSystem::PlaySound("ladderCreak");
			}
			else 
			{
				// theyre not touching a ladder they should just jump
				Jump(ts);
				AudioSystem::StopSound("footsteps");
			}
		}


		break;
	case PlayerState::Jump:

		// the player can move while jumping and falling
		Move(ts);

		// add in the fall multiplier to make the player fall down faster than it went up
		// makes jumping more satisfying
		if (m_Rigidbody->GetVelocity().y > 0)
			m_Rigidbody->ChangeVelocity(Physics::Gravity * m_FallMultiplier * (float)ts);


		// check if weve landed on the ground
		if (m_OnGround)
		{
			// return to the grounded state
			if (!(m_OnJumpThrough && !m_CanLandOnPlatform))
			{
				m_State = PlayerState::Grounded;
				AudioSystem::PlaySound("jumpImpact");
			}
		}

		
		break;
	case PlayerState::Crawl:

		// the player can move while crawling
		Move(ts);
		
		// check the player is attempting to stop crawling
		if (!Input::IsKeyDown(sf::Keyboard::LControl))
		{
			EndCrawl();
			// end crawl may not actually make the player stop crawling
			// depending on the space around the player
			// so we need to check if it worked or not
			if (m_State != PlayerState::Crawl)
			{
				// dont do any other crawling activities if were not crawling anymore
				m_Animator->SetCurrentAnimation("idle");
				AudioSystem::StopSound("crawlingFootsteps");
				break;
			}
		}


		// the player moves slower while crawling
		m_Rigidbody->SetVelocity({ m_Rigidbody->GetVelocity().x * m_ClimbHorizontalFactor, m_Rigidbody->GetVelocity().y });
		// set the animation depending on the velocity
		if (fabsf(m_Rigidbody->GetVelocity().x) > 50.0f)
		{
			m_Animator->SetCurrentAnimation("crawl");
			AudioSystem::PlaySound("crawlingFootsteps", false);
		}
		else
		{
			m_Animator->SetCurrentAnimation("idleCrawl");
			AudioSystem::StopSound("crawlingFootsteps");
		}


		// the player can jump from crawing position
		if (Input::IsKeyDown(sf::Keyboard::W))
		{
			// but they most stop crawling first
			EndCrawl();
			// and they can only jump if they have space to stand up
			if (m_State != PlayerState::Crawl)
			{
				// if standing up was succesful
				// then jump
				Jump(ts);
				AudioSystem::StopSound("crawlingFootsteps");
			}
		}


		break;
	case PlayerState::Climb:

		// the player can move while climbing
		Move(ts);

		// let the player climb up the ladder
		if (Input::IsKeyDown(sf::Keyboard::W))
		{
			// the player moves slower while on a ladder
			m_Rigidbody->SetVelocity({ m_Rigidbody->GetVelocity().x * m_ClimbHorizontalFactor, -m_ClimbSpeed });
		}
		// is the player trying to slide down the ladder?
		else if (Input::IsKeyDown(sf::Keyboard::S))
		{
			// fall down the ladder
			m_State = PlayerState::Jump;
			m_Animator->SetCurrentAnimation("jump");
			AudioSystem::StopSound("ladderCreak");
		}
		else
		{
			// limit the vertical speed
			// the player moves slower while on a ladder

			m_Rigidbody->SetVelocity({ m_Rigidbody->GetVelocity().x * m_ClimbHorizontalFactor, m_ClimbSpeed });

			// check if weve landed on the ground
			if (m_OnGround || m_OnJumpThrough)
			{
				m_State = PlayerState::Grounded;
				m_Animator->SetCurrentAnimation("idle");
				AudioSystem::StopSound("ladderCreak");
			}
		}


		break;
	case PlayerState::Hurting:

		// the player cannot move or jump immediately after being hurt


		if (!m_Animator->GetCurrentAnimation().IsPlaying())
		{
			// the attack animation has finished playing
			// move back to the crawling state if the player was hurt while crawling
			// otherwise move back to grounded state
			if (m_Crawling)
			{
				m_State = PlayerState::Crawl;
				m_Animator->SetCurrentAnimation("idleCrawl");
			}
			else
			{
				m_State = PlayerState::Grounded;
				m_Animator->SetCurrentAnimation("idle");
			}
		}


		break;
	case PlayerState::Dead:
		// the player cannot do much while dead
		break;

	default: break;
	}

	// for debug purposes display the state of the player
	DEBUG_DISPLAY("Player state: " + GetStringFromCurrentState());


	// the animator should flip all animations when the player is facing left
	m_Animator->SetFlipped(!m_FacingRight);

}



void PlayerController::OnTriggerEnter(const Collision& collision)
{					
	// if the player collided with a ladder
	if (GetEntityTag(collision.Other) == "Ladder")
	{
		// hit ladder
		// register another ladder contact
		m_LadderContacts += 1;
		// if the player is in the air, it should start climbing the ladder
		if (!(m_OnGround || m_OnJumpThrough))
		{
			m_State = PlayerState::Climb;
			m_Animator->SetCurrentAnimation("climb");

			// only begin playing the ladder sound if this is the first ladder the player has come into contact with
			if (m_LadderContacts == 1)
				AudioSystem::PlaySound("ladderCreak");
		}

	}
	else if (GetEntityLayer(collision.Other) == "JumpThrough")
	{
		// if we have collided with a jump through platform then register that collision
		m_JumpThroughContactCount++;
	}

}	

void PlayerController::OnTriggerStay(const Collision& collision)
{
	if (GetEntityLayer(collision.Other) == "JumpThrough")
	{
		// if collider bottom is *almost* above other collider top
		// by definition, a collision CANNOT occurr of the bottom is actually above the others top
		// it just needs to be "close enough" to count as landing on the platform

		// we also only want to do it when the player is moving down
		if (collision.GlobalBounds.top + (0.5f * collision.GlobalBounds.height) < collision.OtherGlobalBounds.top && m_Rigidbody->GetVelocity().y > 0)
		{
			if (m_CanLandOnPlatform)
			{
				// standard collision resolution
				// set the bottom of the collider to the top of the platform
				// but a bunch of offsets and stuff need to be taken into account
				m_Transform->SetPosition({ m_Transform->GetPosition().x,
					collision.OtherGlobalBounds.top - collision.GlobalBounds.height + m_Transform->GetPosition().y - collision.GlobalBounds.top });
				m_Rigidbody->SetVelocity({ m_Rigidbody->GetVelocity().x, 0.0f });
				// register the player is standing on the ground, and on a jump through platform (so we know we can drop through the ground)
				m_OnGround = true;
				m_OnJumpThrough = true;
			}
		}
	}
}

void PlayerController::OnTriggerExit(const std::pair<Entity, ColliderID>& other)
{
	// the player has left a ladder
	if (GetEntityTag(other.first) == "Ladder")
	{
		// left ladder
		m_LadderContacts -= 1;
		// check if we have left all ladders
		if (m_LadderContacts == 0 && m_State == PlayerState::Climb)
		{
			// go back to grounded state
			m_State = PlayerState::Grounded;
			AudioSystem::StopSound("ladderCreak");
		}

	}
	else if (GetEntityLayer(other.first) == "JumpThrough")
	{
		// detect if we are no longer in contact with any jump through platforms
		m_JumpThroughContactCount--;
		if (!m_JumpThroughContactCount)
			m_OnJumpThrough = false;
	}
}



void PlayerController::Move(float ts)
{
	// dont let the player move right if its against the wall and facing right
	if (Input::IsKeyDown(sf::Keyboard::D) && !(m_AgainstWall && m_FacingRight))
	{
		m_Rigidbody->SetVelocity({ m_MoveSpeed, m_Rigidbody->GetVelocity().y });
		m_FacingRight = true;
	}
	// dont let the player move left if it is against the wall and facing left
	if (Input::IsKeyDown(sf::Keyboard::A) && !(m_AgainstWall && !m_FacingRight))
	{
		m_Rigidbody->SetVelocity({ -m_MoveSpeed, m_Rigidbody->GetVelocity().y });
		m_FacingRight = false;
	}
}

void PlayerController::Jump(float ts)
{
	// give the player a big velocity boost upwards
	m_Rigidbody->ChangeVelocity({ 0.0f, -m_JumpPower });
	// the player is no loinger on the ground
	m_OnGround = false;

	// change the state and the animation
	m_State = PlayerState::Jump;
	m_Animator->SetCurrentAnimation("jump");

	// play the jump sound
	AudioSystem::PlaySound("jump");
}




void PlayerController::StartCrawl()
{
	// the players hitbox needs readjusted
	m_Collider->Reset(m_CrawlingHitbox);
	m_Transform->Translate({ -16.0f, 32.0f });
	m_Light->SetPosition({ 32.0f, 16.0f });

	// make sure we can actually fit in the space
	if (Physics::BoxCast(m_Collider->GetGlobalBounds(), m_GroundLayerMask).first)
	{
		// we cannot fit in the gap
		// go back to standing up
		EndCrawl();
	}
	else
	{
		// change all of the physics cast points to their crawling varieties
		m_LeftCastPoint = { 5, 10 };
		m_RightCastPoint = { 59, 10 };
		m_BottomCastPoint = { 5, 32 };

		m_HorizontalCastSize = { 54, 0.5f };
		m_VerticalCastSize = { 0.5f, 22 };

		// set the state to crawling
		m_State = PlayerState::Crawl;
		m_Crawling = true;
	}
}

void PlayerController::EndCrawl()
{
	// change the hitbox size and reposition the player
	m_Collider->Reset(m_StandingHitbox);
	m_Transform->Translate({ 16, -32 });
	m_Light->SetPosition({ 16.0f, 32.0f });
	
	// make sure we can actually fit in the space
	if (Physics::BoxCast(m_Collider->GetGlobalBounds(), m_GroundLayerMask).first)
	{
		// we cannot fit in the gap
		// go back to crawling
		StartCrawl();
	}
	else
	{
		// change all of the physics cast points back to the standing varieties
		m_LeftCastPoint = { 7, 16 };
		m_RightCastPoint = { 25, 16 };
		m_BottomCastPoint = { 8, 64 };

		m_HorizontalCastSize = { 17, 0.5f };
		m_VerticalCastSize = { 0.5f, 48 };

		m_State = PlayerState::Grounded;
		m_Crawling = false;
	}
}




void PlayerController::Hurt(bool toTheRight)
{
	// face towards the threat
	m_FacingRight = !toTheRight;

	// apply damage to the player (which in turn updates the GUI)
	m_StatsController->Damage();
	// make the scene light flash red
	m_LightAnimator->BeginAnimate(sf::Color::Red, 0.8f);

	// check if the player has now passed
	if (m_StatsController->IsDead())
	{
		// the player has died :(
		m_Animator->SetCurrentAnimation("die");
		// stop them from moving
		m_Rigidbody->SetVelocity({ 0, 0 });

		// camera effects!
		// give it a good shake
		// and zoom it in on the players body
		m_CameraController->ShakeCamera(0.7f, 7.5f);
		m_CameraController->ZoomCamera(1.0f, 0.3f);

		// the player is dead
		m_State = PlayerState::Dead;
	}
	else
	{
		// if were crawling then just make the player not move
		// playing the hurt animation will look werid
		if (m_State == PlayerState::Crawl)
		{
			// apply the knockback velocity
			m_Rigidbody->SetVelocity({ (toTheRight ? m_HurtBounceVelocity : -m_HurtBounceVelocity) / m_ClimbHorizontalFactor, 0 });
			m_Animator->SetCurrentAnimation("idleCrawl");
		}
		else
		{
			// apply the knockback velocity and play the hurt animation
			m_Rigidbody->SetVelocity({ toTheRight ? m_HurtBounceVelocity : -m_HurtBounceVelocity, 0 });
			m_Animator->SetCurrentAnimation("hurt");
		}
		// shake the camera for neat effect
		m_CameraController->ShakeCamera(0.3f, 4.5f);

		// switch to hurting state
		m_State = PlayerState::Hurting;
	}
	
}



void PlayerController::Reset()
{
	// reset all of the players state variables back to their initial variables
	bool m_OnGround = false;
	bool m_OnJumpThrough = false;
	int m_JumpThroughContactCount = 0;

	bool m_AgainstWall = false;
	bool m_CanLandOnPlatform = true;
	bool m_Crawling = false;

	// move the player back to its starting position
	m_Transform->SetPosition({ 0, -200 });
}

std::string PlayerController::GetStringFromCurrentState()
{
	// debug function that converts the players state into a human readable string
	switch (m_State)
	{
	case PlayerState::Grounded:		return "Grounded";	break;
	case PlayerState::Jump:			return "Jump";		break;
	case PlayerState::Crawl:		return "Crawl";		break;
	case PlayerState::Climb:		return "Climb";		break;
	case PlayerState::Hurting:		return "Hurting";	break;
	case PlayerState::Dead:			return "Dead";		break;
	default:						return "Unknown";	break;
	}
}
