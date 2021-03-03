#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

class PlayerController : public ScriptableEntity
{
public:
	void Start() override
	{
		// do something when the game starts
		m_Transform = &GetComponent<Transform>();
		m_Rigidbody = &GetComponent<Rigidbody>();
		m_Animator = &GetComponent<Animator>();
	}

	void Update(Timestep ts) override
	{
		// do something every frame

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

		if (Input::IsKeyDown(sf::Keyboard::W))
		{
			if (fabsf(m_Rigidbody->Velocity.y) < 0.01f)
			{
				m_Rigidbody->Velocity.y -= m_JumpPower;
			}
		}
		

		if (m_Rigidbody->Velocity.y > 0)
		{
			m_Rigidbody->Velocity += Physics::Gravity * m_FallMultiplier *(float)ts;
		}

		m_Animator->Flip = !m_FacingRight;
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

		DEBUG_DISPLAY("Player position", m_Rigidbody->Position);
		DEBUG_DISPLAY("Player velocity", m_Rigidbody->Velocity);
		DEBUG_DISPLAY("Player on ground", m_OnGround);
	}

	void OnCollisionEnter(Collision collision) override
	{
		m_OnGround = true;
	}

	void OnCollisionExit(Entity other) override
	{
		m_OnGround = false;
	}

private:
	Transform* m_Transform = nullptr;
	Rigidbody* m_Rigidbody = nullptr;
	Animator* m_Animator = nullptr;

	bool m_OnGround = false;

	float m_Move = 0;
	float m_Friction = 14.0f;

	float m_MoveSpeed = 250.0f;
	float m_JumpPower = 450.0f;

	float m_FallMultiplier = 0.4f;

	bool m_FacingRight = true;
};
