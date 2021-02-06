#pragma once

#include "../SFMLEngine.h"

using namespace SFMLEngine;

class MoveByWASD : public ScriptableEntity
{
public:
	void Start() override
	{
		// do something when the game starts
		m_Transform = &GetComponent<Transform>();
	}

	void Update(Timestep ts) override
	{
		// do something every frame

		if (Input::IsKeyDown(sf::Keyboard::D))
			m_Transform->Position += ts * m_MoveSpeed * sf::Vector2f(1, 0);
		if (Input::IsKeyDown(sf::Keyboard::A))
			m_Transform->Position += ts * m_MoveSpeed * sf::Vector2f(-1, 0);
		if (Input::IsKeyDown(sf::Keyboard::S))
			m_Transform->Position += ts * m_MoveSpeed * sf::Vector2f(0, 1);
		if (Input::IsKeyDown(sf::Keyboard::W))
			m_Transform->Position += ts * m_MoveSpeed * sf::Vector2f(0, -1);
		
	}

private:
	Transform* m_Transform = nullptr;

	float m_MoveSpeed = 250.0f;
};
