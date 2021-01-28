#pragma once

#include "../SFMLEngine.h"

using namespace SFMLEngine;

class EntityScript : public ScriptableEntity
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
		m_Transform->Position += (float)ts * sf::Vector2f(50, 20);
	}

private:
	Transform* m_Transform;

};
