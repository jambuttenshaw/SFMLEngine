#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

class SmoothFollowPlayer : public ScriptableEntity
{
public:
	void Start() override
	{
		m_Transform = &GetComponent<Transform>();

		m_PlayerTransform = &GetComponent<Transform>(GetEntitiesWithTag("Player")[0]);
	}

	void Update(Timestep ts) override
	{
		
		if (m_Following)
		{
			m_Transform->Position = Math::Lerp(m_Transform->Position, m_PlayerTransform->Position, m_SmoothSpeed * ts);
			if (Math::SquareMagnitude(m_Transform->Position - m_PlayerTransform->Position) < 10)
			{
				m_Following = false;
			}
		}
		else if (Math::SquareMagnitude(m_Transform->Position - m_PlayerTransform->Position) > m_FollowRadius)
		{
			m_Following = true;
		}
	}

	void SetPlayerTransform(Transform* playerTransform) { m_PlayerTransform = playerTransform; }

private:
	Transform* m_Transform = nullptr;

	Transform* m_PlayerTransform = nullptr;

	bool m_Following = false;

	float m_SmoothSpeed = 3;
	float m_FollowRadius = 7500.0f;
};
