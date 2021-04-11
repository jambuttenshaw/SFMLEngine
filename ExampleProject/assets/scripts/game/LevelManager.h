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

		m_PlayerController = &GetNativeScript<PlayerController>(GetEntitiesWithTag("Player")[0]);
		m_CameraController = &GetNativeScript<CameraController>(GetEntitiesWithTag("MainCamera")[0]);
	}

	void Update(Timestep ts) override
	{
		if (m_FadingIn)
		{
			// set the opacity of the fade in image
			m_FadeProgress += ts * m_FadeSpeed;
			if (m_FadeProgress >= 1)
			{
				// the image has fully faded in
				// load the level
				LoadLevel();

				m_FadingIn = false;
				m_FadingOut = true;
			}
			else
			{
				m_TransitionImage->SetColor({ 255, 255, 255, sf::Uint8(m_FadeProgress * 255) });
			}
		}
		else if (m_FadingOut)
		{
			m_FadeProgress -= ts * m_FadeSpeed;
			if (m_FadeProgress <= 0)
			{
				Destroy(m_TransitionEntity);
				m_TransitionImage = nullptr;

				m_FadingOut = false;
			}
			else
			{
				m_TransitionImage->SetColor({ 255, 255, 255, sf::Uint8(m_FadeProgress * 255) });
			}
		}
	}

	void OnTriggerEnter(const Collision& collision) override
	{
		if (GetEntityTag(collision.Other) == "LevelEnd")
		{
			if (m_LoadedLevel1) return;

			m_FadingIn = true;
			float m_FadeProgress = 0.0f;
		
			CreateTransitionEntity();
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


	void CreateTransitionEntity()
	{
		m_TransitionEntity = CreateEntity();


		GUIImage i{ Texture::Create("assets/textures/black.png") };
		i.SetColor({ 255, 255, 255, 0 });
		AddComponent(m_TransitionEntity, i);


		GUITransform t{ {0.0f, 0.0f }, GUIElementType::Image };
		sf::Vector2f windowSize = static_cast<sf::Vector2f>(Application::GetWindowSize());
		t.SetScale({ windowSize.x / 256.0f, windowSize.y / 256.0f });
		AddComponent(m_TransitionEntity, t);


		m_TransitionImage = &GetComponent<GUIImage>(m_TransitionEntity);
	}

private:
	Application* m_App = nullptr;
	Scene* m_MainScene = nullptr;

	bool m_FadingIn = false;
	bool m_FadingOut = false;
	float m_FadeProgress = 0.0f;
	float m_FadeSpeed = 3.0f;

	bool m_LoadedLevel1 = false;

	PlayerController* m_PlayerController = nullptr;
	CameraController* m_CameraController = nullptr;


	Entity m_TransitionEntity = INVALID_ENTITY_ID;
	GUIImage* m_TransitionImage = nullptr;
};
