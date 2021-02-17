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

		if (Input::IsKeyDown(sf::Keyboard::D))
		{
			m_Rigidbody->Position += ts * m_MoveSpeed * sf::Vector2f(1, 0);
			m_FacingRight = true;
		}
		if (Input::IsKeyDown(sf::Keyboard::A))
		{
			m_Rigidbody->Position += ts * m_MoveSpeed * sf::Vector2f(-1, 0);
			m_FacingRight = false;
		}	

		if (Input::IsKeyDown(sf::Keyboard::W))
		{
			if (fabsf(m_Rigidbody->Velocity.y) < 0.1f)
				m_Rigidbody->Velocity += ts * m_JumpPower * sf::Vector2f(0, -1);
		}

		if (m_Rigidbody->Velocity.y < 0)
		{
			m_Rigidbody->Velocity.y += 750 * (m_FallMultiplier - 1) * ts;
		}
	}

private:
	Transform* m_Transform = nullptr;
	Rigidbody* m_Rigidbody = nullptr;

	float m_MoveSpeed = 250.0f;
	float m_JumpPower = 70000.0f;

	float m_FallMultiplier = 1.5;

	bool m_FacingRight = true;
};
