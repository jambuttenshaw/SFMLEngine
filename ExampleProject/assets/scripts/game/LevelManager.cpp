#include "LevelManager.h"

#include "../../ExampleProject/src/DeathScreen.h"
#include "../../ExampleProject/src/levels/Level1.h"
#include "../../ExampleProject/src/levels/Tutorial.h"


void LevelManager::Start()
{
	m_App = Application::GetApplicationHandle();

	m_PlayerController = &GetNativeScript<PlayerController>(GetEntitiesWithTag("Player")[0]);
	m_CameraController = &GetNativeScript<CameraController>(GetEntitiesWithTag("MainCamera")[0]);

	m_Fader = CreateEntity();
	m_FaderScript = &AddNativeScript<Fader>(m_Fader);
	m_FaderScript->SetFadeState(Fader::State::FadeOut);
	m_FaderScript->SetFadeSpeed(1.0f);
}

void LevelManager::Update(float ts)
{
	if (m_FadingIn)
	{
		// check if the fade in has complete
		m_FadingIn = !m_FaderScript->FadeComplete();

		// if it has finished
		if (!m_FadingIn)
		{
			// the image has fully faded in
			// perform the action
			switch (m_Action)
			{
			case Action::None: break;
			case Action::LoadLevel1: LoadLevel(); break;
			case Action::ToDeathScreen: LoadDeathScreen(); break;
			}

			// fade back out
			m_FadingIn = false;
			m_FadingOut = true;
			m_FaderScript->SetFadeState(Fader::State::FadeOut);
		}
	}
	else if (m_FadingOut)
	{
		m_FadingOut = !m_FaderScript->FadeComplete();

		if (!m_FadingOut)
		{
			m_FadingOut = false;
		}
	}
}

void LevelManager::OnPlayerDeath()
{
	m_FadingIn = true;
	m_Action = Action::ToDeathScreen;
	m_FaderScript->SetFadeState(Fader::State::FadeIn);
}

void LevelManager::OnTriggerEnter(const Collision& collision)
{
	if (GetEntityTag(collision.Other) == "LevelEnd")
	{
		if (m_LoadedLevel1) return;

		m_FadingIn = true;
		m_Action = Action::LoadLevel1;
		m_FaderScript->SetFadeState(Fader::State::FadeIn);
	}
}

void LevelManager::LoadLevel()
{
	// delete the current level scene
	// we only want to delete the scene that doesn't contain the player, HUD etc
	for (Scene* scene : m_App->GetLoadedScenes())
	{
		if (scene != m_MainScene)
		{
			m_App->DeleteScene(scene);
		}
	}
	m_App->LoadScene<Level1>(LoadSceneMode::Additive);

	// reset the player and camera
	m_PlayerController->Reset();
	// snaps the camera to the players position
	m_CameraController->ImmediateReset();

	m_LoadedLevel1 = true;
}

void LevelManager::LoadDeathScreen()
{
	// load the death screen
	// use single load scene mode so that the game scene gets deleted
	m_App->LoadScene<DeathScreen>(LoadSceneMode::Single);
}
