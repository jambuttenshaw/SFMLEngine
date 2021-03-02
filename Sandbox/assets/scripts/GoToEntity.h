#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

class GoToEntity : public ScriptableEntity
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
		m_Transform->Position = m_Target->Position + m_Offset;
	}

	void SetTarget(Transform* t) { m_Target = t; }

private:
	const sf::Vector2f m_Offset = {16, 32};

	Transform* m_Transform = nullptr;
	Transform* m_Target = nullptr;
};
