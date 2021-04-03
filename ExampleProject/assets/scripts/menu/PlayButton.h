#pragma once

#include <SFMLEngine.h>
#include "Button.h"

using namespace SFMLEngine;


class MainScene;


class PlayButton : public Button
{
public:
	void Start() override
	{
		m_SceneLight = &GetComponent<PointLight>(GetEntitiesWithTag("SceneLight")[0]);
		m_InitialRange = m_SceneLight->GetRange();

	}

	void Update(Timestep ts) override
	{
		Button::Update(ts);

		if (m_Fading)
		{
			m_FadeTimer += ts;
			m_SceneLight->SetRange(Math::Lerp(m_InitialRange, 0.0f, m_FadeTimer / m_FadeTime));
			if (m_FadeTimer >= m_FadeTime)
			{
				// time to switch scene
				Application::GetApplicationHandle()->LoadScene<MainScene>();
			}
		}
		
	}

	void OnClick() override
	{
		m_Fading = true;
	}

private:
	PointLight* m_SceneLight;

	float m_InitialRange = 0.0f;

	bool m_Fading = false;
	float m_FadeTime = 0.5f;
	float m_FadeTimer = 0.0f;

};
