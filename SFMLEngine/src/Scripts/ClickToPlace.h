#pragma once

#include "../SFMLEngine.h"

using namespace SFMLEngine;

class ClickToPlace : public ScriptableEntity
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
		bool mouseDown = Input::IsMouseButtonDown(sf::Mouse::Left);
		if (mouseDown && !m_Clicked)
		{
			m_Transform->Position = Input::GetMouseWorldPos();
			m_Clicked = true;
		}
		else if (!mouseDown && m_Clicked)
		{
			m_Clicked = false;
		}
	}

private:
	Transform* m_Transform = nullptr;
	bool m_Clicked = false;
};
