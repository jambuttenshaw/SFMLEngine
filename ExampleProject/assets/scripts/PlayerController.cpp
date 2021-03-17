#include "PlayerController.h"

void PlayerController::Start()
{
	m_Transform = &GetComponent<Transform>();
	m_Rigidbody = &GetComponent<Rigidbody>();
	m_Animator = &GetComponent<Animator>();
}

void PlayerController::Update(Timestep ts)
{

	m_OnGround = fabsf(m_Rigidbody->Velocity.y) < 0.001f;

	if (!m_Attacking)
	{
		// player can attack
		if (Input::IsKeyPressed(sf::Keyboard::Space) && m_OnGround)
		{
			m_Attacking = true;
			m_Animator->SetCurrentAnimation("punch");
		}
		else
		{
			// animations need to be done first, as we want to set the animation based
			// on the data of the player AFTER the physics is applied
			if (m_OnGround)
			{
				if (fabsf(m_Rigidbody->Velocity.x) > 100.0f)
					m_Animator->SetCurrentAnimation("run");
				else
					m_Animator->SetCurrentAnimation("idle");
			}
			else
			{
				if (m_OnLadder)
					m_Animator->SetCurrentAnimation("climb");
				else
					m_Animator->SetCurrentAnimation("jump");
			}

			// then we can move the player
			Move(ts);
			Jump(ts);
			// when update exists, physics will be applied internally by the engine
		}
	}
	else if (!m_Animator->GetCurrentAnimation().Playing)
	{
		// the attack animation has finished playing
		m_Attacking = false;
	}

	m_Animator->Flip = !m_FacingRight;

	DEBUG_DISPLAY("Player position", m_Rigidbody->Position);
	DEBUG_DISPLAY("Player velocity", m_Rigidbody->Velocity);
	DEBUG_DISPLAY("Player on ground", m_OnGround);
	DEBUG_DISPLAY("Player on ladder", m_OnLadder);
}


void PlayerController::OnColliderEnter(const Collision& collision)
{

}
void PlayerController::OnColliderExit(Entity other)
{

}


void PlayerController::OnTriggerEnter(const Collision& collision)
{						 
	if (GetTag(collision.Other) == "Ladder")
	{
		// hit ladder
		m_OnLadder = true;
	}
}						 
void PlayerController::OnTriggerExit(Entity other)
{
	if (GetTag(other) == "Ladder")
	{
		// left ladder
		m_OnLadder = false;
	}
}



void PlayerController::Move(Timestep ts)
{
	m_Rigidbody->Velocity.x = Math::Lerp(m_Rigidbody->Velocity.x, 0.0f, m_Friction * ts);
	if (Input::IsKeyDown(sf::Keyboard::D))
	{
		m_Rigidbody->Velocity.x = m_MoveSpeed;
		m_FacingRight = true;
	}
	if (Input::IsKeyDown(sf::Keyboard::A))
	{
		m_Rigidbody->Velocity.x = -m_MoveSpeed;
		m_FacingRight = false;
	}
	if (m_OnLadder && !m_OnGround)
		m_Rigidbody->Velocity.x *= m_ClimbHorizontalFactor;
}

void PlayerController::Jump(Timestep ts)
{
	if (Input::IsKeyDown(sf::Keyboard::W))
	{
		if (m_OnLadder)
		{
			m_Rigidbody->Velocity.y = -m_ClimbSpeed;
			m_OnGround = false;
		}
		else if (fabsf(m_Rigidbody->Velocity.y) < 0.01f)
		{
			m_Rigidbody->Velocity.y -= m_JumpPower;
			m_OnGround = false;
		}
	}
	if (m_Rigidbody->Velocity.y > 0)
	{
		if (m_OnLadder)
		{
			m_Rigidbody->Velocity.y = m_ClimbSpeed;
		}
		else
		{
			m_Rigidbody->Velocity += Physics::Gravity * m_FallMultiplier * (float)ts;
		}
	}
}
