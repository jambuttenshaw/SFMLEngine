#pragma once

#include <SFMLEngine.h>
using namespace SFMLEngine;


class KnobController : public ScriptableEntity
{
public:

	void Start() override
	{
		m_Transform = &GetComponent<Transform>();
		m_Image = &GetComponent<SpriteRenderer>();
	}

	void Update(float ts) override
	{
		m_CollidingWithMouse = m_Transform->GetLocalToWorldTransformMatrix().transformRect(m_Image->GetSpriteObject().getGlobalBounds()).contains(Input::GetMouseWorldPos());
		
		if (Input::IsMouseButtonDown(sf::Mouse::Left))
		{
			if (m_CollidingWithMouse)
			{
				m_Move = true;
			}
		}
		else
		{
			m_Move = false;
		}

		if (m_Move)
		{
			float x = Math::Clamp(Input::GetMouseWorldPos().x, m_MinX, m_MaxX); // take the sprites size into account
			m_Transform->SetPosition({ x, m_Transform->GetPosition().y });
		}
			
	}


	void SetBoundaries(float min, float max)
	{
		m_MinX = min;
		m_MaxX = max;
	}


	float GetPercent()
	{
		return (m_Transform->GetPosition().x - m_MinX) / (m_MaxX - m_MinX);
	}


private:
	Transform* m_Transform = nullptr;
	SpriteRenderer* m_Image = nullptr;

	bool m_CollidingWithMouse = false;
	bool m_Move = false;

	float m_MinX = 0;
	float m_MaxX = 0;

};