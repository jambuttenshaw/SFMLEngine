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

	void Update(float ts) override
	{
		sf::Vector2f aim = Math::Lerp(m_Target, Input::GetMouseWorldPos(), 0.03f);
		if (m_MovingToTarget)
		{
			m_Transform->SetPosition(Math::Lerp(m_Transform->GetPosition(), aim, 10.0f * ts));
			
			if (Math::SquareMagnitude(m_Transform->GetPosition() - aim) < 1.0f)
			{
				m_MovingToTarget = false;
				m_Transform->SetPosition(aim);
			}
			
		}
		else
		{
			m_Transform->SetPosition(aim);
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
