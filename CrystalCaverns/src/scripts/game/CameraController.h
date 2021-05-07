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
				m_ShakeMovement = m_GlobalShakeMagnitudeMultiplier * m_ShakeMagnitude * fac * fac * fac * Math::RandomUnitVector();
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


		// zooming of the camera
		if (m_Zooming)
		{
			m_ZoomTimer -= ts;
			if (m_ZoomTimer <= 0)
			{
				m_Zooming = false;
			}
			else
			{
				// zoom timer goes from total zoom time to 0
				// that is why the inputs to lerp are swapped
				// (lerping from target to initial instead of the usual intial to target)
				m_Camera->SetZoom(Math::Lerp(m_ZoomTarget, m_InitialZoom, m_ZoomTimer / m_TotalZoomTime));
			}
		}
	}

	void ImmediateReset() { m_FollowPos = m_PlayerTransform->GetPosition(); }

	void SetPlayerTransform(Transform* playerTransform) { m_PlayerTransform = playerTransform; }


	void ShakeCamera(float duration, float magnitude)
	{
		m_Shaking = true;
		m_ShakeTimer = duration * m_GlobalShakeDurationMultiplier;
		m_ShakeMagnitudeNormalizationFactor = duration;
		m_ShakeMagnitude = magnitude;
	}

	void ZoomCamera(float duration, float zoomLevel)
	{
		m_Zooming = true;
		m_ZoomTimer = duration;
		m_TotalZoomTime = duration;

		m_ZoomTarget = zoomLevel;
		m_InitialZoom = m_Camera->GetZoom();
	}

	void ResetZoom(float value)
	{
		m_Camera->SetZoom(value);
		m_InitialZoom = value;
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


	bool m_Zooming = false;
	float m_ZoomTimer = 0.0f;
	float m_TotalZoomTime = 0.0f;
	float m_ZoomTarget = 0.0f;
	float m_InitialZoom = 0.0f;


	const float m_GlobalShakeMagnitudeMultiplier = 1.0f;
	const float m_GlobalShakeDurationMultiplier = 1.3f;
};
