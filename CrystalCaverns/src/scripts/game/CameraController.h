#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

class CameraController : public ScriptableEntity
{
public:
	void Start() override
	{
		m_Transform = &GetComponent<Transform>();
		m_Camera = &GetComponent<Camera>();

		m_PlayerTransform = &GetComponent<Transform>(GetEntitiesWithTag("Player")[0]);
	}

	void Update(float ts) override
	{
		if (m_Shaking)
		{
			m_ShakeTimer -= ts;
			if (m_ShakeTimer <= 0)
			{
				m_Shaking = false;
				m_ShakeMovement = { 0, 0 };
			}
			else
			{
				float fac = (m_ShakeTimer / m_ShakeMagnitudeNormalizationFactor);
				m_ShakeMovement = m_ShakeMagnitude * fac * fac * Math::RandomUnitVector();
			}
		}

		if (m_Following)
		{
			m_FollowPos = (Math::Lerp(m_FollowPos, m_PlayerTransform->GetPosition(), m_SmoothSpeed * ts));
			if (Math::SquareMagnitude(m_FollowPos - m_PlayerTransform->GetPosition()) < 10)
			{
				m_Following = false;
			}
		}
		else if (Math::SquareMagnitude(m_FollowPos - m_PlayerTransform->GetPosition()) > m_FollowRadius)
		{
			m_Following = true;
		}

		m_Transform->SetPosition(m_FollowPos + m_ShakeMovement);
	}

	void ImmediateReset() { m_FollowPos = m_PlayerTransform->GetPosition(); }

	void SetPlayerTransform(Transform* playerTransform) { m_PlayerTransform = playerTransform; }


	void ShakeCamera(float duration, float magnitude)
	{
		m_Shaking = true;
		m_ShakeTimer = duration;
		m_ShakeMagnitudeNormalizationFactor = duration;
		m_ShakeMagnitude = magnitude;
	}


private:
	Transform* m_Transform = nullptr;
	Camera* m_Camera = nullptr;

	Transform* m_PlayerTransform = nullptr;

	bool m_Following = false;
	sf::Vector2f m_FollowPos;

	float m_SmoothSpeed = 3;
	float m_FollowRadius = 5000.0f;


	// camera effects
	bool m_Shaking = false;
	float m_ShakeTimer = 0.0f;
	sf::Vector2f m_ShakeMovement;
	float m_ShakeMagnitude = 5.0f;
	float m_ShakeMagnitudeNormalizationFactor = 1.0f;
};
