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
		// check if the mouse pointer is over the knob
		// this is done by transforming the bounding box of the image into screen space
		// and then checking if the mouse pos is inside the screen space rect
		m_CollidingWithMouse = m_Transform->GetLocalToWorldTransformMatrix().transformRect(m_Image->GetSpriteObject().getGlobalBounds()).contains(Input::GetMouseWorldPos());
		
		// once the mouse is clicked, the slider follows the mouse until it is released
		// even if the mouse is no longer over the slider
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

		// if the slider is moving
		if (m_Move)
		{
			// the sliders x position follows the mouse, but is clamped between the max and the min edges of the slider
			float x = Math::Clamp(Input::GetMouseWorldPos().x, m_MinX, m_MaxX); // take the sprites size into account
			// update the knobs position
			m_Transform->SetPosition({ x, m_Transform->GetPosition().y });
		}
			
	}


	void SetBoundaries(float min, float max)
	{
		// set the edges of the slider
		m_MinX = min;
		m_MaxX = max;
	}


	float GetPercent()
	{
		// find out what percentage the knob is along the slider
		// this is in the scale 0 to 1
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