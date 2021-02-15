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

		if (Input::IsKeyDown(sf::Keyboard::Right))
			m_Rigidbody->Position += ts * m_MoveSpeed * sf::Vector2f(1, 0);
		if (Input::IsKeyDown(sf::Keyboard::Left))
			m_Rigidbody->Position += ts * m_MoveSpeed * sf::Vector2f(-1, 0);

		if (Input::IsKeyDown(sf::Keyboard::Up))
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

	float m_MoveSpeed = 400.0f;
	float m_JumpPower = 75000.0f;

	float m_FallMultiplier = 1.5;
};
