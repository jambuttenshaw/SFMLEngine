#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

class MainMenu;


class SplashScreenAnimator : public ScriptableEntity
{
	// the specific stages in the animation
	enum class AnimationState
	{
		None,
		Wait,
		PointLightMove,
		DirectionalLightIn,
		DirectionalLightOut,
		End
	};

public:

	void Start() override
	{
		m_Transform = &GetComponent<Transform>();
		m_DirLight = &GetComponent<DirectionalLight>();

		// set up the first state of the animation
		WaitThenDo(0, AnimationState::PointLightMove, m_PointLightMoveTime);

		// play the splash screen audio
		AudioSystem::LoadSound("SplashScreenSound", "assets/audio/splashScreenSound.ogg");
		AudioSystem::SetRelativeToListener("SplashScreenSound", false);
		AudioSystem::PlaySound("SplashScreenSound");
	}

	void Update(float ts) override
	{
		// always decrement the timer
		m_Timer -= ts;

		switch (m_State)
		{
		case AnimationState::None:	break; // do nothing
		case AnimationState::Wait:
			// wait until the timer has ran out
			if (m_Timer <= 0)
			{
				// then move onto the next state
				m_State = m_NextState;
				m_Timer = m_NextTime;
			}

			break;
		case AnimationState::PointLightMove:
			// move the point light across the screen
			m_Transform->SetPosition({ Math::Lerp(-800.0f, 800.0f, 1 - m_Timer / m_PointLightMoveTime), 0.0f });
			if (m_Timer <= 0)
			{
				// once this stage has finished, the directional light should be faded in
				WaitThenDo(0, AnimationState::DirectionalLightIn, m_DirLightFadeTime);
			}

			break;
		case AnimationState::DirectionalLightIn:
			// slowly interpolate the intensity of the directional light
			m_DirLight->SetIntensity(Math::Lerp(0.0f, 1.0f, 1 - m_Timer / m_DirLightFadeTime));
			// once this state has finished the directional light should be faded out after a short wait
			if (m_Timer <= 0)
				WaitThenDo(0.5f, AnimationState::DirectionalLightOut, m_DirLightFadeTime);

			break;
		case AnimationState::DirectionalLightOut:
			// fade out the intensity of the light
			m_DirLight->SetIntensity(Math::Lerp(1.0f, 0.0f, 1 - m_Timer / m_DirLightFadeTime));
			// wait a small amount of time then end the splash screen sequence
			if (m_Timer <= 0)
				WaitThenDo(1, AnimationState::End, 0);

			break;
		case AnimationState::End:

			// play the menu music before loading the menu scene
			AudioSystem::PlayMusic();

			// load the menu
			Application::GetApplicationHandle()->LoadScene<MainMenu>(LoadSceneMode::Single);

			// just set the state to none since nothing happens anymore
			m_State = AnimationState::None;

			break;
		}
	}

private:

	void WaitThenDo(float time, AnimationState next, float nextTime)
	{
		// sets up the waiting state
		m_State = AnimationState::Wait;
		m_Timer = time;

		// sets up the state that happens after the waiting is done
		m_NextState = next;
		m_NextTime = nextTime;
	}


private:
	Transform* m_Transform = nullptr;
	DirectionalLight* m_DirLight = nullptr;

	AnimationState m_State = AnimationState::None;
	AnimationState m_NextState = AnimationState::None;
	float m_Timer = 0.0f;
	float m_NextTime = 0.0f;

	// these control how long things take in the sequence
	const float m_PointLightMoveTime = 2.0f;
	const float m_DirLightFadeTime = 0.7f;

};
