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
	}

	void Update(Timestep ts) override
	{
		// do something every frame

		m_Move = Math::Lerp(m_Move, 0, m_Friction * ts);
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
			if (fabsf(m_Rigidbody->Velocity.y) < 0.1f)
				m_Rigidbody->Velocity += ts * m_JumpPower * sf::Vector2f(0, -1);
		}

		if (m_Rigidbody->Velocity.y < 0)
		{
			m_Rigidbody->Velocity += Physics::Gravity * (m_FallMultiplier - 1) * (float)ts;
		}
	}

private:
	Transform* m_Transform = nullptr;
	Rigidbody* m_Rigidbody = nullptr;

	float m_Move = 0;
	float m_Friction = 14.0f;

	float m_MoveSpeed = 250.0f;
	float m_JumpPower = 70000.0f;

	float m_FallMultiplier = 1.5;

	bool m_FacingRight = true;
};
