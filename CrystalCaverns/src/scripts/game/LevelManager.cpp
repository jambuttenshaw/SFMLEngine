#include "LevelManager.h"

#include "DeathScreen.h"
#include "WinScreen.h"
#include "levels/Level1.h"
#include "levels/Tutorial.h"


#include "DepthFinder.h"
#include "CrystalCollector.h"
#include "PlayerStatsController.h"


void LevelManager::Start()
{
	// this class uses the application a lot so its helpful to store a handle to it
	m_App = Application::GetApplicationHandle();

	m_PlayerController = &GetNativeScript<PlayerController>(GetEntitiesWithTag("Player")[0]);
	m_CameraController = &GetNativeScript<CameraController>(GetEntitiesWithTag("MainCamera")[0]);

	// the level manager makes use of a fader entity to smoothly transition between levels
	m_Fader = CreateEntity();
	m_FaderScript = &AddNativeScript<Fader>(m_Fader);
	m_FaderScript->SetFadeState(Fader::State::FadeOut);
	m_FaderScript->SetFadeSpeed(1.0f);

	// we need to find out if level 1 has been loaded, to decide if we should load level 1 or the win screen
	m_LoadedLevel1 = DataStore::RetrieveData<bool>("loadedLevel1");
	m_EnteredTrigger = false;
}

void LevelManager::Update(float ts)
{
	// is the fader fading in
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
			case Action::ToWinScreen: LoadWinScreen(); break;
			}

			// fade back out
			m_FadingIn = false;
			m_FadingOut = true;
			m_FaderScript->SetFadeState(Fader::State::FadeOut);
		}
	}
	else if (m_FadingOut)
	{
		// update the fading out flag
		m_FadingOut = !m_FaderScript->FadeComplete();
	}
}

void LevelManager::OnPlayerDeath()
{
	// when the player has died we want to fade the fader in and progress to the death screen
	m_FadingIn = true;
	m_Action = Action::ToDeathScreen;
	m_FaderScript->SetFadeState(Fader::State::FadeIn);
}

void LevelManager::OnTriggerEnter(const Collision& collision)
{
	// the player has finished the level!
	if (GetEntityTag(collision.Other) == "LevelEnd")
	{
		// check we havent already registered this collision
		if (!m_EnteredTrigger)
		{
			m_EnteredTrigger = true;

			// if level 1 has already been loaded in this playthrough
			if (*m_LoadedLevel1)
			{
				// we must progress to the win screen
				m_FadingIn = true;
				m_Action = Action::ToWinScreen;
				m_FaderScript->SetFadeState(Fader::State::FadeIn);
			}
			else
			{
				// level 1 has not been played yet so we load that in
				m_FadingIn = true;
				m_Action = Action::LoadLevel1;
				m_FaderScript->SetFadeState(Fader::State::FadeIn);
			}
		}
	}
}



void LevelManager::LoadLevel()
{
	// delete the current level scene
	// we only want to delete the scene that doesn't contain the player, HUD etc
	for (Scene* scene : m_App->GetLoadedScenes())
	{
		// delete any scene that is not the main scene
		// this should only be the tutorial scene, but we do not have a pointer to it so this method will do
		if (scene != m_MainScene)
		{
			m_App->DeleteScene(scene);
		}
	}
	// load in level 1 with additive scene loading
	// so we do not replace the player etc
	m_App->LoadScene<Level1>(LoadSceneMode::Additive);

	// reset the player and camera
	m_PlayerController->Reset();
	// snaps the camera to the players position
	m_CameraController->ImmediateReset();

	// flag level 1 as having been loaded
	*m_LoadedLevel1 = true;
	// allow us to trigger events when colliding with the level end again
	m_EnteredTrigger = false;

}

void LevelManager::LoadDeathScreen()
{
	// load the death screen
	// use single load scene mode so that the game scene gets deleted
	m_App->LoadScene<DeathScreen>(LoadSceneMode::Single);
}

void LevelManager::LoadWinScreen()
{
	// first of all retrieve all the players score info from its scripts
	// as it is required by the win screen
	GetNativeScript<CrystalCollector>().AssignPlayerData();
	GetNativeScript<PlayerStatsController>().AssignPlayerData();
	
	GetNativeScript<DepthFinder>(GetEntitiesWithTag("DepthBar")[0]).AssignPlayerData();


	// load the win screen
	// use single load scene mode so that the game scene gets deleted
	m_App->LoadScene<WinScreen>(LoadSceneMode::Single);
}
