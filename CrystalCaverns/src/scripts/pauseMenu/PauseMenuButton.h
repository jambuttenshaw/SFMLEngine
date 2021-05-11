#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;


class PauseMenuButton : public ScriptableEntity
{
	void Start() override
	{
		m_Image = &GetComponent<GUIImage>();
		m_Transform = &GetComponent<GUITransform>();
	}


	void Update(float ts) override
	{
		// since the pause menu exists in screen space rather than world space we cannot use the collision system to detect collisions
		// we can just use the transform of this button to find out the rectangle of the image in screen space
		// and see if the mouse pointer lies within that rectangle
		sf::Vector2f mousePos{ Input::GetMouseScreenPos() };
		m_CollidingWithMouse = m_Transform->GetTransformMatrix().transformRect(m_Image->GetSpriteObject().getGlobalBounds()).contains(mousePos);

		// we want to trigger the event after the mouse is released
		if (Input::IsMouseButtonDown(sf::Mouse::Left))
		{
			// signify that we are waiting for the mouse to be released
			if (!m_Holding) m_Holding = true;
		}
		else if (m_Holding)
		{
			// the mouse has been released
			// check if we are colliding with the mouse
			if (m_CollidingWithMouse)
			{
				// play the click sound and trigger the click event
				AudioSystem::PlaySound("buttonClick");
				OnClick();
			}

			m_Holding = false;
		}
	}

	// to be implemented by individual buttons
	virtual void OnClick() = 0;


private:
	GUIImage* m_Image = nullptr;
	GUITransform* m_Transform = nullptr;

	bool m_CollidingWithMouse = false;
	bool m_Holding = false;

};


// the 3 buttons that the pause menu contains
class PResumeButton : public PauseMenuButton
{
	void OnClick() override;
};

class PMenuButton : public PauseMenuButton
{
	void OnClick() override;
};

class PQuitButton : public PauseMenuButton
{
	void OnClick() override;
};