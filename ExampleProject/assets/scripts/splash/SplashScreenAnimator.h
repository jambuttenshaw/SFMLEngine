#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

class MainMenu;


class SplashScreenAnimator : public ScriptableEntity
{

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

		WaitThenDo(0, AnimationState::PointLightMove, m_PointLightMoveTime);

		AudioSystem::LoadSound("SplashScreenSound", "assets/audio/splashScreenSound.ogg");
		AudioSystem::SetRelativeToListener("SplashScreenSound", false);
		AudioSystem::PlaySound("SplashScreenSound");
	}

	void Update(float ts) override
	{
		m_Timer -= ts;

		switch (m_State)
		{
		case AnimationState::None:	break;
		case AnimationState::Wait:
			if (m_Timer <= 0)
			{
				m_State = m_NextState;
				m_Timer = m_NextTime;
			}

			break;
		case AnimationState::PointLightMove:
			m_Transform->SetPosition({ Math::Lerp(-800.0f, 800.0f, 1 - m_Timer / m_PointLightMoveTime), 0.0f });
			if (m_Timer <= 0)
			{
				WaitThenDo(0, AnimationState::DirectionalLightIn, m_DirLightFadeTime);
			}

			break;
		case AnimationState::DirectionalLightIn:
			m_DirLight->SetIntensity(Math::Lerp(0.0f, 1.0f, 1 - m_Timer / m_DirLightFadeTime));

			if (m_Timer <= 0)
				WaitThenDo(0.5f, AnimationState::DirectionalLightOut, m_DirLightFadeTime);

			break;
		case AnimationState::DirectionalLightOut:
			m_DirLight->SetIntensity(Math::Lerp(1.0f, 0.0f, 1 - m_Timer / m_DirLightFadeTime));
			if (m_Timer <= 0)
				WaitThenDo(1, AnimationState::End, 0);

			break;
		case AnimationState::End:

			// play the menu music before loading the menu scene
			AudioSystem::PlayMusic();

			Application::GetApplicationHandle()->LoadScene<MainMenu>(LoadSceneMode::Single);
			m_State = AnimationState::None;

			break;
		}
	}

private:

	void WaitThenDo(float time, AnimationState next, float nextTime)
	{
		m_State = AnimationState::Wait;
		m_Timer = time;

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


	float m_PointLightMoveTime = 2.0f;
	float m_DirLightFadeTime = 0.7f;

};
