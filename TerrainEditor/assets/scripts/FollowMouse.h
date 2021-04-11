#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

class FollowMouse : public ScriptableEntity
{
public:
	void Start() override
	{
		// do something when the game starts
		m_Transform = &GetComponent<Transform>();
	}

	void Update(Timestep ts) override
	{
		m_Transform->SetPosition(Input::GetMouseWorldPos() + m_Offset);
	}

private:
	Transform* m_Transform = nullptr;

	sf::Vector2f m_Offset = { -8, -8 };
};
