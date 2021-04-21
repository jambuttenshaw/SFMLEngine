#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

class MainMenu;


class SplashScreenAnimator : public ScriptableEntity
{

	enum AnimationState
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

		WaitThenDo(0, PointLightMove, m_PointLightMoveTime);
	}

	void Update(float ts) override
	{
		m_Timer -= ts;

		switch (m_State)
		{
		case None:	break;
		case Wait:
			if (m_Timer <= 0)
			{
				m_State = m_NextState;
				m_Timer = m_NextTime;
			}

			break;
		case PointLightMove:
			m_Transform->SetPosition({ Math::Lerp(-800.0f, 800.0f, 1 - m_Timer / m_PointLightMoveTime), 0.0f });
			if (m_Timer <= 0)
				WaitThenDo(0, DirectionalLightIn, m_DirLightFadeTime);

			break;
		case DirectionalLightIn:
			m_DirLight->SetIntensity(Math::Lerp(0.0f, 1.0f, 1 - m_Timer / m_DirLightFadeTime));

			if (m_Timer <= 0)
				WaitThenDo(0.5f, DirectionalLightOut, m_DirLightFadeTime);

			break;
		case DirectionalLightOut:
			m_DirLight->SetIntensity(Math::Lerp(1.0f, 0.0f, 1 - m_Timer / m_DirLightFadeTime));
			if (m_Timer <= 0)
				WaitThenDo(0, End, 0);

			break;
		case End:
			Application::GetApplicationHandle()->LoadScene<MainMenu>(LoadSceneMode::Single);
			m_State = None;

			break;
		}
	}

private:

	void WaitThenDo(float time, AnimationState next, float nextTime)
	{
		m_State = Wait;
		m_Timer = time;

		m_NextState = next;
		m_NextTime = nextTime;
	}


private:
	Transform* m_Transform = nullptr;
	DirectionalLight* m_DirLight = nullptr;

	AnimationState m_State = None;
	AnimationState m_NextState = None;
	float m_Timer = 0.0f;
	float m_NextTime = 0.0f;


	float m_PointLightMoveTime = 2.0f;
	float m_DirLightFadeTime = 0.7f;

};
