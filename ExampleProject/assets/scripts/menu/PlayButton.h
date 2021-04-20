#pragma once

#include <SFMLEngine.h>
#include "Button.h"

using namespace SFMLEngine;


class MainScene;
class Tutorial;
class Level1;


class PlayButton : public Button
{
public:
	void Start() override
	{
		Entity lightEntity = GetEntitiesWithTag("SceneLight")[0];

		m_SceneLight = &GetComponent<PointLight>(lightEntity);
		m_DirLight = &GetComponent<DirectionalLight>(lightEntity);

		m_InitialRange = m_SceneLight->GetRange();
		m_InitialIntensity = m_DirLight->GetIntensity();
	}

	void Update(Timestep ts) override
	{
		Button::Update(ts);

		if (m_Fading)
		{
			m_FadeTimer += ts;

			m_SceneLight->SetRange(Math::Lerp(m_InitialRange, 0.0f, m_FadeTimer / m_FadeTime));
			m_DirLight->SetIntensity(Math::Lerp(m_InitialIntensity, 0.0f, m_FadeTimer / m_FadeTime));

			if (m_FadeTimer >= m_FadeTime)
			{
				// time to switch scene
				Application::GetApplicationHandle()->LoadScene<Tutorial>(LoadSceneMode::Additive);
				Application::GetApplicationHandle()->LoadScene<MainScene>(LoadSceneMode::Single);
			}
		}
		
	}

	void OnClick() override
	{
		m_Fading = true;
	}

private:
	PointLight* m_SceneLight;
	DirectionalLight* m_DirLight;

	float m_InitialRange = 0.0f;
	float m_InitialIntensity = 0.0f;

	bool m_Fading = false;
	float m_FadeTime = 0.5f;
	float m_FadeTimer = 0.0f;

};
