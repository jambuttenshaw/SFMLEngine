#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

/*
The camera controller positions the camera to smoothly follow the player
it is also in charge of cool effects like the zooms and camera shakes
*/
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
		// if the camera should be shaking
		if (m_Shaking)
		{
			// count down until the timer is up
			m_ShakeTimer -= ts;
			if (m_ShakeTimer <= 0)
			{
				// reset the shake
				m_Shaking = false;
				m_ShakeMovement = { 0, 0 };
			}
			else
			{
				// fac is the progress through the shake
				// this makes the shake lose intensity over time
				float fac = (m_ShakeTimer / m_ShakeMagnitudeNormalizationFactor);
				// the shake is ultimately produced by a random unit vector scaled by a series of scalars
				m_ShakeMovement = m_GlobalShakeMagnitudeMultiplier * m_ShakeMagnitude * fac * fac * fac * Math::RandomUnitVector();
			}
		}

		// if the camera should be following the player
		if (m_Following)
		{
			// find the position the camera should move to this frame
			m_FollowPos = (Math::Lerp(m_FollowPos, m_PlayerTransform->GetPosition(), m_SmoothSpeed * ts));
			// if the camera is really close to the player we do not want to move it anymore
			if (Math::SquareMagnitude(m_FollowPos - m_PlayerTransform->GetPosition()) < 10)
			{
				m_Following = false;
			}
		}
		// if we are not following, check if the player is far enough away to start following again
		else if (Math::SquareMagnitude(m_FollowPos - m_PlayerTransform->GetPosition()) > m_FollowRadius)
		{
			m_Following = true;
		}

		// position the camera taking into account the follow position and the movement from the shake
		m_Transform->SetPosition(m_FollowPos + m_ShakeMovement);


		// zooming of the camera
		if (m_Zooming)
		{
			// decrement timer
			m_ZoomTimer -= ts;
			if (m_ZoomTimer <= 0)
			{
				// stop zooming
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

	// snap the camera back to the player
	void ImmediateReset() { m_FollowPos = m_PlayerTransform->GetPosition(); }

	void ShakeCamera(float duration, float magnitude)
	{
		// set the camera to be shaking for the specified duration at the specified magnitude
		m_Shaking = true;
		m_ShakeTimer = duration * m_GlobalShakeDurationMultiplier;
		m_ShakeMagnitudeNormalizationFactor = duration;
		m_ShakeMagnitude = magnitude;
	}

	void ZoomCamera(float duration, float zoomLevel)
	{
		// zoom the camera in to the specified zoom level in the specified duration
		m_Zooming = true;
		m_ZoomTimer = duration;
		m_TotalZoomTime = duration;

		m_ZoomTarget = zoomLevel;
		m_InitialZoom = m_Camera->GetZoom();
	}

	void ResetZoom(float value)
	{
		// reset the zoom to its initial value
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

	// zoom effects
	bool m_Zooming = false;
	float m_ZoomTimer = 0.0f;
	float m_TotalZoomTime = 0.0f;
	float m_ZoomTarget = 0.0f;
	float m_InitialZoom = 0.0f;

	// constant modifiers
	const float m_GlobalShakeMagnitudeMultiplier = 1.0f;
	const float m_GlobalShakeDurationMultiplier = 1.3f;
};
