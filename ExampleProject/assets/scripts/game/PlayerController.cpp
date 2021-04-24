#include "PlayerController.h"

#include "PauseScript.h"


void PlayerController::Start()
{
	m_Transform = &GetComponent<Transform>();
	m_Rigidbody = &GetComponent<Rigidbody>();
	m_Animator = &GetComponent<Animator>();
	m_Collider = &GetComponent<BoxCollider>();

	m_StatsController = &GetNativeScript<PlayerStatsController>();

	m_CameraController = &GetNativeScript<CameraController>(GetEntitiesWithTag("MainCamera")[0]);

	m_GroundLayerMask = LayerManager::GetLayer("Ground") | LayerManager::GetLayer("JumpThrough");

	m_LeftCastPoint = { 7, 16 };
	m_RightCastPoint = { 25, 16 };
	m_BottomCastPoint = { 8, 64 };

	m_HorizontalCastSize = { 17, 0.5f };
	m_VerticalCastSize = { 0.5f, 48 };



	// load in the players sounds
	AudioSystem::LoadSound("ladderCreak", "assets/audio/ladderCreak.ogg");
	AudioSystem::SetLooping("ladderCreak", true);
	
	AudioSystem::LoadSound("footsteps", "assets/audio/footsteps.ogg");
	AudioSystem::SetLooping("footsteps", true);

	AudioSystem::LoadSound("jump", "assets/audio/jump.ogg");
	AudioSystem::LoadSound("jumpImpact", "assets/audio/jumpImpact.ogg");
}

void PlayerController::Update(float ts)
{

	// if the game is paused: the player does nothing
	if (PauseScript::IsGamePaused()) return;


	// friction should ALWAYS be applied to the player
	sf::Vector2f oldVel{ m_Rigidbody->GetVelocity() };
	m_Rigidbody->SetVelocity({ Math::Lerp(oldVel.x, 0.0f, m_Friction * ts), oldVel.y });


	// all user input response needs to be below this
	if (m_State == PlayerState::Dead) return;


	if (!(m_OnJumpThrough && m_CanLandOnPlatform))
		m_OnGround = Physics::BoxCast({ m_Transform->GetPosition() + m_BottomCastPoint, m_HorizontalCastSize }, m_GroundLayerMask).first;

	if (m_FacingRight)
		m_AgainstWall = Physics::BoxCast({ m_Transform->GetPosition() + m_RightCastPoint, m_VerticalCastSize }, m_GroundLayerMask).first;
	else
		m_AgainstWall = Physics::BoxCast({ m_Transform->GetPosition() + m_LeftCastPoint, m_VerticalCastSize }, m_GroundLayerMask).first;



	if (m_CanLandOnPlatform)
	{
		if (Input::IsKeyDown(sf::Keyboard::S))
		{
			m_CanLandOnPlatform = false;


			// the player should move to jump state after falling down through a platform
			if (m_State == PlayerState::Crawl)
				EndCrawl();

			m_Animator->SetCurrentAnimation("jump");
			m_State = PlayerState::Jump;
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
			AudioSystem::StopSound("footsteps");
			m_Animator->SetCurrentAnimation("jump");
			m_State = PlayerState::Jump;

			// the player should do no more 'grounded' activities
			break;
		}


		// the player can begin crawling 
		if (Input::IsKeyDown(sf::Keyboard::LControl))
		{
			StartCrawl();
			// dont do any other grounded activities if were now crawling
			if (m_State == PlayerState::Crawl)
			{
				AudioSystem::StopSound("footsteps");
				m_Animator->SetCurrentAnimation("idleCrawl");
				break;
			}
		}


		// the player can move while on the ground
		Move(ts);

		if (fabsf(m_Rigidbody->GetVelocity().x) > 100.0f)
		{
			m_Animator->SetCurrentAnimation("run");
			AudioSystem::PlaySound("footsteps", false);
		}
		else
		{
			m_Animator->SetCurrentAnimation("idle");
			AudioSystem::StopSound("footsteps");
		}
		// make sure to jump after setting the run animation
		// if the player does jump, then we want to change the animation

		// the player can jump while on the ground
		if (Input::IsKeyDown(sf::Keyboard::W))
		{
			if (m_LadderContacts)
			{
				AudioSystem::StopSound("footsteps");

				m_State = PlayerState::Climb;
				m_Animator->SetCurrentAnimation("climb");
				AudioSystem::PlaySound("ladderCreak");
			}
			else 
			{
				Jump(ts);
				AudioSystem::StopSound("footsteps");
			}
		}


		break;
	case PlayerState::Jump:

		// the player can move while jumping and falling
		Move(ts);

		// add in the fall multiplier to make the player fall down faster than it went up
		if (m_Rigidbody->GetVelocity().y > 0)
			m_Rigidbody->ChangeVelocity(Physics::Gravity * m_FallMultiplier * (float)ts);


		// check if weve landed on the ground
		if (m_OnGround)
		{
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
		

		if (!Input::IsKeyDown(sf::Keyboard::LControl))
		{
			EndCrawl();
			// dont do any other crawling activities if were not crawling anymore
			if (m_State != PlayerState::Crawl)
			{
				m_Animator->SetCurrentAnimation("idle");
				break;
			}
		}


		// the player moves slower while crawling
		m_Rigidbody->SetVelocity({ m_Rigidbody->GetVelocity().x * m_ClimbHorizontalFactor, m_Rigidbody->GetVelocity().y });
		if (fabsf(m_Rigidbody->GetVelocity().x) > 50.0f)
			m_Animator->SetCurrentAnimation("crawl");
		else
			m_Animator->SetCurrentAnimation("idleCrawl");


		// the player can jump from crawing position
		if (Input::IsKeyDown(sf::Keyboard::W))
		{
			EndCrawl();
			if (m_State != PlayerState::Crawl)
			{
				// if standing up was succesful
				// then jump
				Jump(ts);
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
			// move back to idle state
			m_State = PlayerState::Grounded;
			m_Animator->SetCurrentAnimation("idle");
		}


		break;
	case PlayerState::Dead:
		// the player cannot do much while dead
		break;

	default: break;
	}


	DEBUG_DISPLAY("Player state: " + GetStringFromCurrentState());


	m_Animator->SetFlipped(!m_FacingRight);

}



void PlayerController::OnTriggerEnter(const Collision& collision)
{					
	if (GetEntityTag(collision.Other) == "Ladder")
	{
		// hit ladder
		m_LadderContacts += 1;
		if (!(m_OnGround || m_OnJumpThrough))
		{
			m_State = PlayerState::Climb;
			m_Animator->SetCurrentAnimation("climb");

			if (m_LadderContacts == 1)
				AudioSystem::PlaySound("ladderCreak");
		}

	}
	else if (GetEntityLayer(collision.Other) == "JumpThrough")
	{
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
		if (collision.GlobalBounds.top + 0.75f * collision.GlobalBounds.height < collision.OtherGlobalBounds.top && m_Rigidbody->GetVelocity().y > 0)
		{
			if (m_CanLandOnPlatform)
			{
				m_Transform->SetPosition({ m_Transform->GetPosition().x,
					collision.OtherGlobalBounds.top - collision.GlobalBounds.height + m_Transform->GetPosition().y - collision.GlobalBounds.top });
				m_Rigidbody->SetVelocity({ m_Rigidbody->GetVelocity().x, 0.0f });
				m_OnGround = true;
				m_OnJumpThrough = true;
			}
		}
	}
}

void PlayerController::OnTriggerExit(const std::pair<Entity, ColliderID>& other)
{
	if (GetEntityTag(other.first) == "Ladder")
	{
		// left ladder
		m_LadderContacts -= 1;
		// check if we have left all ladders
		if (m_LadderContacts == 0 && m_State == PlayerState::Climb)
		{
			m_State = PlayerState::Grounded;
			AudioSystem::StopSound("ladderCreak");
		}

	}
	else if (GetEntityLayer(other.first) == "JumpThrough")
	{
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
	m_Rigidbody->ChangeVelocity({ 0.0f, -m_JumpPower });
	m_OnGround = false;

	m_State = PlayerState::Jump;
	m_Animator->SetCurrentAnimation("jump");

	AudioSystem::PlaySound("jump");
}




void PlayerController::StartCrawl()
{
	// the players hitbox needs readjusted
	m_Collider->Reset(m_CrawlingHitbox);
	m_Transform->Translate({ -16.0f, 32.0f });

	// make sure we can actually fit in the space
	if (Physics::BoxCast(m_Collider->GetGlobalBounds(), m_GroundLayerMask).first)
	{
		EndCrawl();
	}
	else
	{
		m_LeftCastPoint = { 5, 10 };
		m_RightCastPoint = { 59, 10 };
		m_BottomCastPoint = { 5, 32 };

		m_HorizontalCastSize = { 54, 0.5f };
		m_VerticalCastSize = { 0.5f, 22 };

		m_State = PlayerState::Crawl;
	}
}

void PlayerController::EndCrawl()
{
	m_Collider->Reset(m_StandingHitbox);
	m_Transform->Translate({ 16, -32 });
	
	// make sure we can actually fit in the space
	if (Physics::BoxCast(m_Collider->GetGlobalBounds(), m_GroundLayerMask).first)
	{
		StartCrawl();
	}
	else
	{
		m_LeftCastPoint = { 7, 16 };
		m_RightCastPoint = { 25, 16 };
		m_BottomCastPoint = { 8, 64 };

		m_HorizontalCastSize = { 17, 0.5f };
		m_VerticalCastSize = { 0.5f, 48 };

		m_State = PlayerState::Grounded;
	}
}




void PlayerController::Hurt(bool toTheRight)
{
	// face towards the threat
	m_FacingRight = !toTheRight;

	m_StatsController->Damage();

	// check if the player has now passed
	if (m_StatsController->IsDead())
	{
		// the player has died :(
		m_Animator->SetCurrentAnimation("die");
		m_Rigidbody->SetVelocity({ 0, 0 });

		m_CameraController->ShakeCamera(0.7f, 7.5f);


		m_State = PlayerState::Dead;
	}
	else
	{
		// if were crawling then just make the player not move
		if (m_State == PlayerState::Crawl)
		{
			m_Rigidbody->SetVelocity({ (toTheRight ? m_HurtBounceVelocity : -m_HurtBounceVelocity) / m_ClimbHorizontalFactor, 0 });
			m_Animator->SetCurrentAnimation("idleCrawl");
		}
		else
		{
			m_Rigidbody->SetVelocity({ toTheRight ? m_HurtBounceVelocity : -m_HurtBounceVelocity, 0 });
			m_Animator->SetCurrentAnimation("hurt");
		}
		m_CameraController->ShakeCamera(0.3f, 4.5f);


		m_State = PlayerState::Hurting;
	}
	
}



void PlayerController::Reset()
{
	bool m_OnGround = false;
	bool m_OnJumpThrough = false;
	int m_JumpThroughContactCount = 0;

	bool m_AgainstWall = false;
	bool m_CanLandOnPlatform = true;
	bool m_Crawling = false;

	m_Transform->SetPosition({ 0, -200 });
}

std::string PlayerController::GetStringFromCurrentState()
{
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
