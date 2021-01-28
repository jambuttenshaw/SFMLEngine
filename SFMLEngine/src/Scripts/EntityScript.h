#pragma once

#include "../SFMLEngine.h"


class EntityScript : public SFMLEngine::ScriptableEntity
{
public:
	void Start() override
	{
		// do something when the game starts
		m_Transform = &GetComponent<SFMLEngine::Transform>();
	}

	void Update(float timestep) override
	{
		// do something every frame
		m_Transform->Position += sf::Vector2f(1, 0);
	}

private:
	SFMLEngine::Transform* m_Transform;

};
