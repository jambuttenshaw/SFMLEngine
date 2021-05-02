#pragma once

#include <SFMLEngine.h>

#include "menu/PlayButton.h"
#include "menu/QuitButton.h"
#include "menu/OptionsButton.h"
#include "menu/BackButton.h"

#include "menu/GoToMouse.h"
#include "menu/MenuCameraController.h"
#include "menu/FadeInController.h"

#include "menu/OptionsSlider.h"

using namespace SFMLEngine;


class MainMenu : public Scene
{
public:

	void Create() override
	{
		{
			m_PlayButton = CreateEntity();

			AddComponent(m_PlayButton, Transform{ { 0, -64 } });
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
			m_SoundsVolumeSlider = CreateEntity();

			AddComponent(m_SoundsVolumeSlider, Transform{ { -128, 800 } });
			AddComponent(m_SoundsVolumeSlider, SpriteRenderer{
					Texture::Create("assets/textures/buttons/soundsOptionsSlider.png"),
					Material::Create("Lit"),
					0,
					Texture::Create("assets/textures/buttons/soundsOptionsSlider_n.png")
				});
			auto& script = AddNativeScript<OptionsSlider>(m_SoundsVolumeSlider);
			script.SetSliderFunction([](float val) { AudioSystem::SetListenerVolume(val); }, 0, 100);
		}
		{
			m_MusicVolumeSlider = CreateEntity();

			AddComponent(m_MusicVolumeSlider, Transform{ { -128, 928 } });
			AddComponent(m_MusicVolumeSlider, SpriteRenderer{
					Texture::Create("assets/textures/buttons/musicOptionsSlider.png"),
					Material::Create("Lit"),
					0,
					Texture::Create("assets/textures/buttons/musicOptionsSlider_n.png")
				});
			auto& script = AddNativeScript<OptionsSlider>(m_MusicVolumeSlider);
			script.SetSliderFunction([](float val) { AudioSystem::SetMusicVolume(val); }, 0, 100);
		}
		

		{
			m_Ground = CreateEntity();

			AddComponent(m_Ground, Transform{ });

			// add a tilemap

			AddComponent(m_Ground, Tilemap{ TilePalette::LoadFromFile("assets/palettes/terrainPalette.json"), "assets/tilemaps/menu/layer0.json" });
			AddComponent(m_Ground, TilemapRenderer{ Material::Create("Lit"), -1 });
		}


		m_TitleText = CreateEntity();
		AddComponent(m_TitleText, Transform{ { -239.0f, -293.5f } });
		AddComponent(m_TitleText, SpriteRenderer{
			Texture::Create("assets/textures/titleText.png"),
			Material::Create("Lit"),
			0,
			Texture::Create("assets/textures/titleText_n.png") });


		m_TitleTextLight = CreateEntity();
		SetEntityTag(m_TitleTextLight, "TitleLight");
		AddComponent(m_TitleTextLight, Transform{ { 240, 86 }, &GetComponent<Transform>(m_TitleText) });
		AddComponent(m_TitleTextLight, PointLight{ 1.5f, 150.0f, sf::Color(17, 32, 255, 255) });



		m_Light = CreateEntity();
		SetEntityTag(m_Light, "SceneLight");
		AddComponent(m_Light, Transform{ });
		AddComponent(m_Light, PointLight{ 3.0f, 400.0f, sf::Color(219, 113, 114, 255) });
		AddNativeScript<GoToMouse>(m_Light);
		// place both lights onto the same entity
		AddComponent(m_Light, DirectionalLight{ -1.5f, 0.7f, 0.5f, sf::Color(240, 61, 19, 255) });



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

	Entity m_SoundsVolumeSlider = INVALID_ENTITY_ID;
	Entity m_MusicVolumeSlider = INVALID_ENTITY_ID;

	Entity m_Ground = INVALID_ENTITY_ID;

	Entity m_Camera = INVALID_ENTITY_ID;
	Entity m_Light = INVALID_ENTITY_ID;

	Entity m_FadeInController = INVALID_ENTITY_ID;

	Entity m_TitleText = INVALID_ENTITY_ID;
	Entity m_TitleTextLight = INVALID_ENTITY_ID;
};
