#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;


class MenuCameraController : public ScriptableEntity
{
public:
	void Start() override
	{
		m_Transform = &GetComponent<Transform>();
	}

	void Update(Timestep ts) override
	{
		sf::Vector2f aim = Math::Lerp(m_Target, Input::GetMouseWorldPos(), 0.05f);
		if (m_MovingToTarget)
		{
			m_Transform->Position = Math::Lerp(m_Transform->Position, aim, 10.0f * ts);
			
			if (Math::SquareMagnitude(m_Transform->Position - aim) < 1.0f)
			{
				m_MovingToTarget = false;
				m_Transform->Position = aim;
			}
			
		}
		else
		{
			m_Transform->Position = aim;
		}
	}


	void SetTarget(const sf::Vector2f& point)
	{
		m_Target = point; 
		m_MovingToTarget = true;
	}

private:
	Transform* m_Transform = nullptr;

	bool m_MovingToTarget = false;
	sf::Vector2f m_Target{ 0, 0 };

};
