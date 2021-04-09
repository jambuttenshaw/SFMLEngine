#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

class Tutorial;
class Level1;

class LevelManager : public ScriptableEntity
{
public:
	void Start() override
	{
		m_App = Application::GetApplicationHandle();
		Entity player = GetEntitiesWithTag("Player")[0];
		m_PlayerController = &GetNativeScript<PlayerController>(player);
	}

	void OnTriggerEnter(const Collision& collision) override
	{
		if (GetEntityTag(collision.Other) == "LevelEnd")
		{
			if (m_LoadedLevel1) return;


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

			m_LoadedLevel1 = true;
		}
	}

	void SetMainScene(Scene* s) { m_MainScene = s; }

private:
	Application* m_App = nullptr;
	Scene* m_MainScene = nullptr;

	bool m_LoadedLevel1 = false;

	PlayerController* m_PlayerController = nullptr;
};
