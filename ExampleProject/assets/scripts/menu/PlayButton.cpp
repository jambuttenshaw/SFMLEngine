#include "PlayButton.h"

#include "../../ExampleProject/src/MainScene.h"
#include "../../ExampleProject/src/levels/Tutorial.h"


void PlayButton::Start()
{
	Entity lightEntity = GetEntitiesWithTag("SceneLight")[0];

	m_SceneLight = &GetComponent<PointLight>(lightEntity);
	m_DirLight = &GetComponent<DirectionalLight>(lightEntity);

	m_InitialRange = m_SceneLight->GetRange();
	m_InitialIntensity = m_DirLight->GetIntensity();
}

void PlayButton::Update(float ts)
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

void PlayButton::OnClick()
{
	m_Fading = true;
}
