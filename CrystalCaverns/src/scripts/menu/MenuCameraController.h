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
		// the camera follows the mouse around very slightly to make the menu feel more dynamic
		sf::Vector2f aim = Math::Lerp(m_Target, Input::GetMouseWorldPos(), 0.03f);
		// if the camera is currently in transit to a new target
		if (m_MovingToTarget)
		{
			// move towards the target
			m_Transform->SetPosition(Math::Lerp(m_Transform->GetPosition(), aim, 10.0f * ts));
			
			// if we are really close to the target
			if (Math::SquareMagnitude(m_Transform->GetPosition() - aim) < 1.0f)
			{
				// then stop moving towards the target
				m_MovingToTarget = false;
				m_Transform->SetPosition(aim);
			}
			
		}
		else
		{
			// if we are not moving just place the camera at the aim point
			m_Transform->SetPosition(aim);
		}
	}


	void SetTarget(const sf::Vector2f& point)
	{
		// set where the camera should move to
		m_Target = point; 
		m_MovingToTarget = true;
	}

private:
	Transform* m_Transform = nullptr;

	bool m_MovingToTarget = false;
	sf::Vector2f m_Target{ 0, 0 };

};
