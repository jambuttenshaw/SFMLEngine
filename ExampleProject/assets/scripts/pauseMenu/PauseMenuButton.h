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
		sf::Vector2f mousePos{ Input::GetMouseScreenPos() };
		m_CollidingWithMouse = m_Transform->GetTransformMatrix().transformRect(m_Image->GetSpriteObject().getGlobalBounds()).contains(mousePos);

		if (Input::IsMouseButtonDown(sf::Mouse::Left))
		{
			if (!m_Holding) m_Holding = true;
		}
		else if (m_Holding)
		{
			if (m_CollidingWithMouse)
				OnClick();

			m_Holding = false;
		}
	}

	virtual void OnClick() = 0;


private:
	GUIImage* m_Image = nullptr;
	GUITransform* m_Transform = nullptr;

	bool m_CollidingWithMouse = false;
	bool m_Holding = false;

};



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