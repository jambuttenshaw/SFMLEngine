#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;


class Button : public ScriptableEntity
{
public:

	void Update(float ts) override
	{
		// find out if the button is colliding with the mouse
		// perform a circle cast with a really small radius at the mouse's position
		auto [m_CollidingWithMouse, collision] = Physics::CircleCast(Input::GetMouseWorldPos(), 0.1f, Physics::AllMask);
		if (m_CollidingWithMouse)
		{
			// make sure that it collided with this button
			if (collision.Other != GetEntityHandle())
				m_CollidingWithMouse = false;
		}

		if (Input::IsMouseButtonDown(sf::Mouse::Left))
		{
			// if the mouse button is held down set that its being held
			if (!m_Holding) m_Holding = true;
		}
		else if (m_Holding)
		{
			// this will be triggered on the first frame that the mouse is released
			// check if the mouse is colliding with the button
			if (m_CollidingWithMouse)
			{
				// play the click sound
				AudioSystem::PlaySound("buttonClick");
				// call the function that happens when the button is clicked
				OnClick();
			}

			m_Holding = false;
		}
	}

	// to be implemented in the child classes
	virtual void OnClick() = 0;


private:
	bool m_CollidingWithMouse = false;
	bool m_Holding = false;
};
