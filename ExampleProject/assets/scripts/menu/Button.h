#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;


class Button : public ScriptableEntity
{
public:
	void Start() override
	{

	}

	void Update(Timestep ts) override
	{
		auto [m_CollidingWithMouse, collision] = Physics::CircleCast(Input::GetMouseWorldPos(), 0.1f, Physics::AllMask);
		if (m_CollidingWithMouse)
		{
			if (collision.Other != GetEntityHandle())
				m_CollidingWithMouse = false;
		}

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
	bool m_CollidingWithMouse = false;
	bool m_Holding = false;
};
