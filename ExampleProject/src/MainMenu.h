#pragma once

#include <SFMLEngine.h>

#include "menu/PlayButton.h"
#include "menu/QuitButton.h"
#include "menu/OptionsButton.h"
#include "menu/BackButton.h"

#include "menu/GoToMouse.h"
#include "menu/MenuCameraController.h"
#include "menu/FadeInController.h"

using namespace SFMLEngine;


class MainMenu : public Scene
{
public:

	void Create() override
	{

		
		{
			m_PlayButton = CreateEntity();

			AddComponent(m_PlayButton, Transform{ { 0, -96 } });
			AddComponent(m_PlayButton, SpriteRenderer{
					Texture::Create("assets/textures/playButton.png"),
					Material::Create("Lit"),
					0,
					Texture::Create("assets/textures/playButton_n.png")
				});
			
			AddComponent(m_PlayButton, BoxCollider{ {128, 64}, {0, 0} });
			AddComponent(m_PlayButton, ColliderInfo{ ColliderType::Box });

			AddNativeScript<PlayButton>(m_PlayButton);
		}

		
		
		
		{
			m_OptionsButton = CreateEntity();

			AddComponent(m_OptionsButton, Transform{ { -128, 64 } });
			AddComponent(m_OptionsButton, SpriteRenderer{
					Texture::Create("assets/textures/optionsButton.png"),
					Material::Create("Lit"),
					0,
					Texture::Create("assets/textures/optionsButton_n.png")
				});

			AddComponent(m_OptionsButton, BoxCollider{ {160, 64}, {0, 0} });
			AddComponent(m_OptionsButton, ColliderInfo{ ColliderType::Box });

			AddNativeScript<OptionsButton>(m_OptionsButton);
		}

		{
			m_OptionsBackButton = CreateEntity();

			AddComponent(m_OptionsBackButton, Transform{ { -192, 1056 } });
			AddComponent(m_OptionsBackButton, SpriteRenderer{
					Texture::Create("assets/textures/backButton.png"),
					Material::Create("Lit"),
					0,
					Texture::Create("assets/textures/backButton_n.png")
				});

			AddComponent(m_OptionsBackButton, BoxCollider{ {128, 64}, {0, 0} });
			AddComponent(m_OptionsBackButton, ColliderInfo{ ColliderType::Box });

			auto& script = AddNativeScript<BackButton>(m_OptionsBackButton);
		}
		
		
		{
			m_QuitButton = CreateEntity();

			AddComponent(m_QuitButton, Transform{ { 0, 224 } });
			AddComponent(m_QuitButton, SpriteRenderer{
					Texture::Create("assets/textures/quitButton.png"),
					Material::Create("Lit"),
					0,
					Texture::Create("assets/textures/quitButton_n.png")
				});

			AddComponent(m_QuitButton, BoxCollider{ {128, 64}, {0, 0} });
			AddComponent(m_QuitButton, ColliderInfo{ ColliderType::Box });

			AddNativeScript<QuitButton>(m_QuitButton);
		}
		

		{
			m_Ground = CreateEntity();

			AddComponent(m_Ground, Transform{ });

			// add a tilemap

			AddComponent(m_Ground, Tilemap{ TilePalette::LoadFromFile("assets/palettes/terrainPalette.json"), "assets/tilemaps/menu/layer0.json" });
			AddComponent(m_Ground, TilemapRenderer{ Material::Create("LitTilemap"), 0 });
		}


		m_Light = CreateEntity();
		SetEntityTag(m_Light, "SceneLight");
		AddComponent(m_Light, Transform{ });
		AddComponent(m_Light, PointLight{ 3.0f, 400.0f, sf::Color(219, 113, 114, 255) });
		AddNativeScript<GoToMouse>(m_Light);
		// place both lights onto the same entity
		AddComponent(m_Light, DirectionalLight{ -1.5f, 0.7f, 0.3f, sf::Color(200, 61, 19, 255) });



		m_Camera = CreateEntity();
		SetEntityTag(m_Camera, "Camera");

		AddComponent(m_Camera, Transform{ });
		AddComponent(m_Camera, Camera{ });
		AddNativeScript<MenuCameraController>(m_Camera);


		m_FadeInController = CreateEntity();
		AddNativeScript<FadeInController>(m_FadeInController);
	}

private:
	Entity m_PlayButton = INVALID_ENTITY_ID;
	Entity m_QuitButton = INVALID_ENTITY_ID;
	
	Entity m_OptionsButton = INVALID_ENTITY_ID;
	Entity m_OptionsBackButton = INVALID_ENTITY_ID;

	Entity m_Ground = INVALID_ENTITY_ID;

	Entity m_Camera = INVALID_ENTITY_ID;
	Entity m_Light = INVALID_ENTITY_ID;

	Entity m_FadeInController = INVALID_ENTITY_ID;
};
