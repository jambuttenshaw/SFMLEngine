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
	}

	void Update(Timestep ts) override
	{
		if (Input::IsKeyPressed(sf::Keyboard::H))
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
		}
	}

	void SetMainScene(Scene* s) { m_MainScene = s; }

private:
	Application* m_App;
	Scene* m_MainScene;
};
