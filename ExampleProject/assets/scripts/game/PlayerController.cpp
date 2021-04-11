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

	// friction should ALWAYS be applied to the player
	sf::Vector2f oldVel{ m_Rigidbody->GetVelocity() };
	m_Rigidbody->SetVelocity({ Math::Lerp(oldVel.x, 0.0f, m_Friction * ts), oldVel.y });




	if (!(m_OnJumpThrough && m_CanLandOnPlatform))
		m_OnGround = Physics::BoxCast({ m_Transform->GetPosition() + m_BottomCastPoint, m_HorizontalCastSize }, m_GroundLayerMask).first;

	if (m_FacingRight)
		m_AgainstWall = Physics::BoxCast({ m_Transform->GetPosition() + m_RightCastPoint, m_VerticalCastSize }, m_GroundLayerMask).first;
	else
		m_AgainstWall = Physics::BoxCast({ m_Transform->GetPosition() + m_LeftCastPoint, m_VerticalCastSize }, m_GroundLayerMask).first;


	// all user input response needs to be below this
	if (m_Dead) return;



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


	if (m_Hurting)
	{
		if (!m_Animator->GetCurrentAnimation().IsPlaying())
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
				if (fabsf(m_Rigidbody->GetVelocity().x) > 50.0f)
					m_Animator->SetCurrentAnimation("crawl");
				else
					m_Animator->SetCurrentAnimation("idleCrawl");
					
			}
			else
			{
				if (fabsf(m_Rigidbody->GetVelocity().x) > 100.0f)
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

	m_Animator->SetFlipped(!m_FacingRight);

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
	}
	else if (GetEntityLayer(other.first) == "JumpThrough")
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
		m_Rigidbody->SetVelocity({ m_MoveSpeed, m_Rigidbody->GetVelocity().y });
		m_FacingRight = true;
	}
	// dont let the player move left if it is against the wall and facing left
	if (Input::IsKeyDown(sf::Keyboard::A) && !(m_AgainstWall && !m_FacingRight))
	{
		m_Rigidbody->SetVelocity({ -m_MoveSpeed, m_Rigidbody->GetVelocity().y });
		m_FacingRight = false;
	}
	if ((m_LadderContacts && !m_OnGround) || m_Crawling)
		m_Rigidbody->SetVelocity({ m_Rigidbody->GetVelocity().x * m_ClimbHorizontalFactor, m_Rigidbody->GetVelocity().y });
}

void PlayerController::Jump(Timestep ts)
{
	if (Input::IsKeyDown(sf::Keyboard::W))
	{
		if (m_LadderContacts)
		{
			m_Rigidbody->SetVelocity({ m_Rigidbody->GetVelocity().x, -m_ClimbSpeed });
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
					m_Rigidbody->ChangeVelocity({ 0.0f, -m_JumpPower });
					m_OnGround = false;
				}
			}
			else
			{
				m_Rigidbody->ChangeVelocity({ 0.0f, -m_JumpPower });
				m_OnGround = false;
			}
		}
	}
	if (m_Rigidbody->GetVelocity().y > 0)
	{
		if (m_LadderContacts)
		{
			if (Input::IsKeyDown(sf::Keyboard::S))
				m_Sliding = true;
			else
			{
				m_Rigidbody->SetVelocity({ m_Rigidbody->GetVelocity().x, m_ClimbSpeed });
				m_Sliding = false;
			}
		}
		
		else
		{
			m_Rigidbody->ChangeVelocity(Physics::Gravity * m_FallMultiplier * (float)ts);
		}
	}
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

		m_Crawling = true;
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

		m_Crawling = false;
	}
}


void PlayerController::Hurt(bool toTheRight)
{
	m_Hurting = true;
	// face towards the threat
	m_FacingRight = !toTheRight;

	m_StatsController->Damage();

	// check if the player has now passed
	if (m_StatsController->IsDead())
	{
		// the player has died :(
		m_Dead = true;
		m_Animator->SetCurrentAnimation("die");
	}
	else
	{
		// if were crawling then just make the player not move
		if (m_Crawling)
		{
			m_Rigidbody->SetVelocity({ (toTheRight ? m_HurtBounceVelocity : -m_HurtBounceVelocity) / m_ClimbHorizontalFactor, m_Rigidbody->GetVelocity().y });
			m_Animator->SetCurrentAnimation("idleCrawl");
		}
		else
		{
			m_Rigidbody->SetVelocity({ toTheRight ? m_HurtBounceVelocity : -m_HurtBounceVelocity, m_Rigidbody->GetVelocity().y });
			m_Animator->SetCurrentAnimation("hurt");
		}
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
