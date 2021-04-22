#pragma once

#include <SFMLEngine.h>
#include "menu/FadeInController.h"
#include "menu/GoToMouse.h"
#include "deathScreen/DeathScreenButtons.h"


#include "PlayerData.h"


using namespace SFMLEngine;


class WinScreen : public Scene
{
public:

	void Create() override
	{
		{
			m_ReplayButton = CreateEntity();

			AddComponent(m_ReplayButton, Transform{ { -180, 113 } });
			AddComponent(m_ReplayButton, SpriteRenderer{
					Texture::Create("assets/textures/buttons/replayButton.png"),
					Material::Create("Lit"),
					0,
					Texture::Create("assets/textures/buttons/replayButton_n.png"),
				});

			AddComponent(m_ReplayButton, BoxCollider{ {160, 64}, {0, 0} });
			AddComponent(m_ReplayButton, ColliderInfo{ ColliderType::Box });

			AddNativeScript<ReplayButton>(m_ReplayButton);
		}
		{
			m_MenuButton = CreateEntity();

			AddComponent(m_MenuButton, Transform{ { 20, 113 } });
			AddComponent(m_MenuButton, SpriteRenderer{
					Texture::Create("assets/textures/buttons/menuButton.png"),
					Material::Create("Lit"),
					0,
					Texture::Create("assets/textures/buttons/menuButton_n.png"),
				});

			AddComponent(m_MenuButton, BoxCollider{ {128, 64}, {0, 0} });
			AddComponent(m_MenuButton, ColliderInfo{ ColliderType::Box });

			AddNativeScript<ExitToMenuButton>(m_MenuButton);
		}


		m_MainText = CreateEntity();
		AddComponent(m_MainText, Transform{ { -189, -200 } });
		AddComponent(m_MainText, SpriteRenderer{
			Texture::Create("assets/textures/winText.png"),
			Material::Create("Lit"),
			0,
			Texture::Create("assets/textures/winText_n.png"),
			});


		m_ScoreText = CreateEntity();
		AddComponent(m_ScoreText, Transform{ { -189.0f, -36.5f } });
		AddComponent(m_ScoreText, SpriteRenderer{
			Texture::Create("assets/textures/scoreText.png"), Material::Create("Basic"), 0 });


		m_Camera = CreateEntity();
		SetEntityTag(m_Camera, "Camera");
		AddComponent(m_Camera, Transform{ });
		AddComponent(m_Camera, Camera{ });


		m_FadeInController = CreateEntity();
		AddNativeScript<FadeInController>(m_FadeInController);


		m_Light = CreateEntity();
		SetEntityTag(m_Light, "SceneLight");
		AddComponent(m_Light, Transform{ });
		AddComponent(m_Light, PointLight{ 3.0f, 400.0f, sf::Color(219, 113, 114, 255) });
		AddNativeScript<GoToMouse>(m_Light);
		// place both lights onto the same entity
		AddComponent(m_Light, DirectionalLight{ 0.0f, -1.7f, 0.3f, sf::Color(200, 61, 19, 255) });



		// load the player data
		PlayerData* data = DataStore::RetrieveData<PlayerData>("playerData");
	}

private:
	Entity m_MainText = INVALID_ENTITY_ID;
	Entity m_ScoreText = INVALID_ENTITY_ID;
	Entity m_Camera = INVALID_ENTITY_ID;
	Entity m_FadeInController = INVALID_ENTITY_ID;

	Entity m_ReplayButton = INVALID_ENTITY_ID;
	Entity m_MenuButton = INVALID_ENTITY_ID;

	Entity m_Light = INVALID_ENTITY_ID;
};
