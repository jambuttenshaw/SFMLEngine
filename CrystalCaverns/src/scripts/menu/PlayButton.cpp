#include "PlayButton.h"

#include "MainScene.h"
#include "levels/Tutorial.h"


void PlayButton::Start()
{
	Entity lightEntity = GetEntitiesWithTag("SceneLight")[0];

	m_SceneLight = &GetComponent<PointLight>(lightEntity);
	m_DirLight = &GetComponent<DirectionalLight>(lightEntity);

	m_TitleLight = &GetComponent<PointLight>(GetEntitiesWithTag("TitleLight")[0]);

	// save the properties of the scene lights for use later
	m_InitialRange = m_SceneLight->GetRange();
	m_InitialIntensity1 = m_DirLight->GetIntensity();
	m_InitialIntensity2 = m_TitleLight->GetIntensity();
}

void PlayButton::Update(float ts)
{
	// we still need to call the button update too for detecting when the button is clicked
	Button::Update(ts);

	// is the main menu fading out?
	if (m_Fading)
	{
		// increase timer
		m_FadeTimer += ts;

		// interpolate the properties of the lights to make it fade to darkness
		m_SceneLight->SetRange(Math::Lerp(m_InitialRange, 0.0f, m_FadeTimer / m_FadeTime));
		m_DirLight->SetIntensity(Math::Lerp(m_InitialIntensity1, 0.0f, m_FadeTimer / m_FadeTime));
		m_TitleLight->SetIntensity(Math::Lerp(m_InitialIntensity2, 0.0f, m_FadeTimer / m_FadeTime));

		if (m_FadeTimer >= m_FadeTime)
		{
			// the fade has completed

			// load the tutorial
			// time to switch scene
			Application::GetApplicationHandle()->LoadScene<Tutorial>(LoadSceneMode::Additive);
			Application::GetApplicationHandle()->LoadScene<MainScene>(LoadSceneMode::Single);
		}
	}

}

void PlayButton::OnClick()
{
	// when the button is clicked begin fading
	m_Fading = true;
}
