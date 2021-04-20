#pragma once

#include <SFMLEngine.h>

#include "Fader.h"


using namespace SFMLEngine;

class Tutorial;
class Level1;

class LevelManager : public ScriptableEntity
{
public:
	void Start() override
	{
		m_App = Application::GetApplicationHandle();

		m_PlayerController = &GetNativeScript<PlayerController>(GetEntitiesWithTag("Player")[0]);
		m_CameraController = &GetNativeScript<CameraController>(GetEntitiesWithTag("MainCamera")[0]);

		m_Fader = CreateEntity();
		m_FaderScript = &AddNativeScript<Fader>(m_Fader);
		m_FaderScript->SetFadeState(Fader::State::FadeOut);
		m_FaderScript->SetFadeSpeed(1);
	}

	void Update(Timestep ts) override
	{
		if (m_FadingIn)
		{
			// check if the fade in has complete
			m_FadingIn = !m_FaderScript->FadeComplete();

			// if it has finished
			if (!m_FadingIn)
			{
				// the image has fully faded in
				// load the level
				LoadLevel();

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
				Destroy(m_Fader);
				m_FadingOut = false;
			}
		}
	}

	void OnTriggerEnter(const Collision& collision) override
	{
		if (GetEntityTag(collision.Other) == "LevelEnd")
		{
			if (m_LoadedLevel1) return;

			m_FadingIn = true;

			m_Fader = CreateEntity();
			m_FaderScript = &AddNativeScript<Fader>(m_Fader);
			m_FaderScript->SetFadeState(Fader::State::FadeIn);
		}
	}

	void SetMainScene(Scene* s) { m_MainScene = s; }


	void LoadLevel()
	{
		// delete the current level scene
		// very crude method of deleting all scenes that dont contain a player
		for (Scene* scene : m_App->GetLoadedScenes())
		{
			if (scene != m_MainScene)
			{
				m_App->DeleteScene(scene);
			}
		}
		m_App->LoadScene<Level1>(LoadSceneMode::Additive);
		m_PlayerController->Reset();
		// snaps the camera to the players position
		m_CameraController->ImmediateReset();

		m_LoadedLevel1 = true;
	}

private:
	Application* m_App = nullptr;
	Scene* m_MainScene = nullptr;

	bool m_LoadedLevel1 = false;

	bool m_FadingIn = false;
	bool m_FadingOut = false;

	PlayerController* m_PlayerController = nullptr;
	CameraController* m_CameraController = nullptr;


	Entity m_Fader = INVALID_ENTITY_ID;
	Fader* m_FaderScript = nullptr;

};
