#include "PlayerController.h"

void PlayerController::Start()
{
	m_Transform = &GetComponent<Transform>();
	m_Rigidbody = &GetComponent<Rigidbody>();
	m_Animator = &GetComponent<Animator>();
}

void PlayerController::Update(Timestep ts)
{

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
			Move(ts);
			Jump(ts);

			if (m_OnGround)
			{
				if (fabsf(m_Move) > 100.0f)
				{
					m_Animator->SetCurrentAnimation("run");
				}
				else
					m_Animator->SetCurrentAnimation("idle");
			}
			else
			{
				m_Animator->SetCurrentAnimation("jump");
			}
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
}


void PlayerController::OnCollisionEnter(Collision collision)
{
	m_OnGround = true;
}
void PlayerController::OnCollisionExit(Entity other)
{
	m_OnGround = false;
}



void PlayerController::Move(Timestep ts)
{
	m_Move = Math::Lerp(m_Move, 0.0f, m_Friction * ts);
	if (Input::IsKeyDown(sf::Keyboard::D))
	{
		m_Move = m_MoveSpeed;
		m_FacingRight = true;
	}
	if (Input::IsKeyDown(sf::Keyboard::A))
	{
		m_Move = -m_MoveSpeed;
		m_FacingRight = false;
	}
	m_Rigidbody->Position.x += ts * m_Move;
}

void PlayerController::Jump(Timestep ts)
{
	if (Input::IsKeyDown(sf::Keyboard::W))
	{
		if (fabsf(m_Rigidbody->Velocity.y) < 0.01f)
		{
			m_Rigidbody->Velocity.y -= m_JumpPower;
			m_OnGround = false;
		}
	}
	if (m_Rigidbody->Velocity.y > 0)
	{
		m_Rigidbody->Velocity += Physics::Gravity * m_FallMultiplier * (float)ts;
	}
}
