#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

class SmoothFollowPlayer : public ScriptableEntity
{
public:
	void Start() override
	{
		// do something when the game starts
		m_Transform = &GetComponent<Transform>();

		m_PlayerTransform = &GetComponent<Transform>(GetEntitiesWithTag("Player")[0]);
	}

	void Update(Timestep ts) override
	{
		// do something every frame
		m_Transform->Position = Math::Lerp(m_Transform->Position, m_PlayerTransform->Position, m_SmoothSpeed * ts);
	}

	void SetPlayerTransform(Transform* playerTransform) { m_PlayerTransform = playerTransform; }

private:
	Transform* m_Transform = nullptr;

	Transform* m_PlayerTransform = nullptr;

	float m_SmoothSpeed = 5;
};
