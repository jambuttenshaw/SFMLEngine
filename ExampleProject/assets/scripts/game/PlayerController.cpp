#include "PlayerController.h"

void PlayerController::Start()
{
	m_Transform = &GetComponent<Transform>();
	m_Rigidbody = &GetComponent<Rigidbody>();
	m_Animator = &GetComponent<Animator>();
	m_Collider = &GetComponent<BoxCollider>();

	m_StatsController = &GetNativeScript<PlayerStatsController>();

	m_GroundLayerMask = LayerManager::GetLayer("Ground") | LayerManager::GetLayer("JumpThrough");

	m_LeftCastPoint = { 7, 16 };
	m_RightCastPoint = { 25, 16 };
	m_BottomCastPoint = { 8, 64 };

	m_HorizontalCastSize = { 17, 0.5f };
	m_VerticalCastSize = { 0.5f, 48 };
}

void PlayerController::Update(Timestep ts)
{
	if (!(m_OnJumpThrough && m_CanLandOnPlatform))
		m_OnGround = Physics::BoxCast({ m_Transform->Position + m_BottomCastPoint, m_HorizontalCastSize }, m_GroundLayerMask).first;

	if (m_FacingRight)
		m_AgainstWall = Physics::BoxCast({ m_Transform->Position + m_RightCastPoint, m_VerticalCastSize }, m_GroundLayerMask).first;
	else
		m_AgainstWall = Physics::BoxCast({ m_Transform->Position + m_LeftCastPoint, m_VerticalCastSize }, m_GroundLayerMask).first;

	if (m_CanLandOnPlatform)
	{
		if (Input::IsKeyDown(sf::Keyboard::S))
			m_CanLandOnPlatform = false;
	}
	else
	{
		// we are no longer on a jump through and S is no longer held down
		// we are able to land on another jump through
		if (!m_OnJumpThrough && !Input::IsKeyDown(sf::Keyboard::S))
			m_CanLandOnPlatform = true;
	}


	if (Input::IsKeyDown(sf::Keyboard::LControl) && m_OnGround)
	{
		if (!m_Crawling)
			StartCrawl();
	}
	else
	{
		if (m_Crawling)
			EndCrawl();
	}


	// friction should ALWAYS be applied to the player
	m_Rigidbody->Velocity.x = Math::Lerp(m_Rigidbody->Velocity.x, 0.0f, m_Friction * ts);



	if (m_Hurting)
	{
		if (!m_Animator->GetCurrentAnimation().Playing)
		{
			// the attack animation has finished playing
			m_Hurting = false;
		}
	}
	else
	{
		Move(ts);
		Jump(ts);



		if (m_OnGround)
		{
			if (m_Crawling)
			{
				if (fabsf(m_Rigidbody->Velocity.x) > 50.0f)
					m_Animator->SetCurrentAnimation("crawl");
				else
					m_Animator->SetCurrentAnimation("idleCrawl");
					
			}
			else
			{
				if (fabsf(m_Rigidbody->Velocity.x) > 100.0f)
					m_Animator->SetCurrentAnimation("run");
				else
					m_Animator->SetCurrentAnimation("idle");
			}
		}
		else
		{
			if (m_LadderContacts && !m_Sliding)
				m_Animator->SetCurrentAnimation("climb");
			else
				m_Animator->SetCurrentAnimation("jump");
		}
	}

	m_Animator->Flip = !m_FacingRight;

}


void PlayerController::OnColliderEnter(const Collision& collision)
{

}

void PlayerController::OnColliderExit(Entity other)
{

}


void PlayerController::OnTriggerEnter(const Collision& collision)
{					
	if (GetEntityTag(collision.Other) == "Ladder")
	{
		// hit ladder
		m_LadderContacts += 1;
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
		if (collision.GlobalBounds.top + 0.75f * collision.GlobalBounds.height < collision.OtherGlobalBounds.top && m_Rigidbody->Velocity.y > 0)
		{
			if (m_CanLandOnPlatform)
			{
				m_Transform->Position.y = collision.OtherGlobalBounds.top - collision.GlobalBounds.height + m_Transform->Position.y - collision.GlobalBounds.top;
				m_Rigidbody->Velocity.y = 0;
				m_OnGround = true;
				m_OnJumpThrough = true;
			}
		}
	}
}

void PlayerController::OnTriggerExit(Entity other)
{
	if (GetEntityTag(other) == "Ladder")
	{
		// left ladder
		m_LadderContacts -= 1;
	}
	else if (GetEntityLayer(other) == "JumpThrough")
	{
		m_JumpThroughContactCount--;
		if (!m_JumpThroughContactCount)
			m_OnJumpThrough = false;
	}
}



void PlayerController::Move(Timestep ts)
{
	// dont let the player move right if its against the wall and facing right
	if (Input::IsKeyDown(sf::Keyboard::D) && !(m_AgainstWall && m_FacingRight))
	{
		m_Rigidbody->Velocity.x = m_MoveSpeed;
		m_FacingRight = true;
	}
	// dont let the player move left if it is against the wall and facing left
	if (Input::IsKeyDown(sf::Keyboard::A) && !(m_AgainstWall && !m_FacingRight))
	{
		m_Rigidbody->Velocity.x = -m_MoveSpeed;
		m_FacingRight = false;
	}
	if ((m_LadderContacts && !m_OnGround) || m_Crawling)
		m_Rigidbody->Velocity.x *= m_ClimbHorizontalFactor;
}

void PlayerController::Jump(Timestep ts)
{
	if (Input::IsKeyDown(sf::Keyboard::W))
	{
		if (m_LadderContacts)
		{
			m_Rigidbody->Velocity.y = -m_ClimbSpeed;
			m_OnGround = false;
		}
		else if (m_OnGround)
		{
			if (m_Crawling)
			{
				EndCrawl();
				if (!m_Crawling)
				{
					// if standing up was succesful
					// then jump
					m_Rigidbody->Velocity.y -= m_JumpPower;
					m_OnGround = false;
				}
			}
			else
			{
				m_Rigidbody->Velocity.y -= m_JumpPower;
				m_OnGround = false;
			}
		}
	}
	if (m_Rigidbody->Velocity.y > 0)
	{
		if (m_LadderContacts)
		{
			if (Input::IsKeyDown(sf::Keyboard::S))
				m_Sliding = true;
			else
			{
				m_Rigidbody->Velocity.y = m_ClimbSpeed;
				m_Sliding = false;
			}
		}
		
		else
		{
			m_Rigidbody->Velocity += Physics::Gravity * m_FallMultiplier * (float)ts;
		}
	}
}


void PlayerController::StartCrawl()
{
	// the players hitbox needs readjusted
	m_Collider->Reset(m_CrawlingHitbox);
	m_Transform->Position.x += m_FacingRight ? -16 : -16;
	m_Transform->Position.y += 32;

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

		m_Crawling = true;
	}
}

void PlayerController::EndCrawl()
{
	m_Collider->Reset(m_StandingHitbox);
	m_Transform->Position.x += m_FacingRight ? 16 : 16;
	m_Transform->Position.y -= 32;
	
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

		m_Crawling = false;
	}
}


void PlayerController::Hurt(bool toTheRight)
{
	m_Hurting = true;
	// face towards the threat
	m_FacingRight = !toTheRight;


	// if were crawling then just make the player not move
	if (m_Crawling)
	{
		m_Rigidbody->Velocity.x = (toTheRight ? m_HurtBounceVelocity : -m_HurtBounceVelocity) / m_ClimbHorizontalFactor;
		m_Animator->SetCurrentAnimation("idleCrawl");
	}
	else
	{
		m_Rigidbody->Velocity.x = toTheRight ? m_HurtBounceVelocity : -m_HurtBounceVelocity;
		m_Animator->SetCurrentAnimation("hurt");
	}

	m_StatsController->Damage(10);
}