#pragma once

#include <SFMLEngine.h>
#include "menu/FadeInController.h"
#include "menu/GoToMouse.h"
#include "deathScreen/DeathScreenButtons.h"

using namespace SFMLEngine;


class DeathScreen : public Scene
{
public:

	void Create() override
	{
		// the replay button and menu button are implemented identically to WinScreen.h
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

		// create text saying the players died
		m_MainText = CreateEntity();
		AddComponent(m_MainText, Transform{ { -140, -52 } });
		AddComponent(m_MainText, SpriteRenderer{
			Texture::Create("assets/textures/deathScreen.png"),
			Material::Create("Lit"),
			0,
			Texture::Create("assets/textures/deathScreen_n.png"),
			});

		// add the scene camera
		// nothing fancy this time
		m_Camera = CreateEntity();
		SetEntityTag(m_Camera, "Camera");
		AddComponent(m_Camera, Transform{ });
		AddComponent(m_Camera, Camera{ });


		// we should fade into the death screen so create a fade in controller entity (same as the menu)
		m_FadeInController = CreateEntity();
		AddNativeScript<FadeInController>(m_FadeInController);

		// and add a scene light
		// this is also the same as the main menu
		// make it follow the mouse for cool effect
		m_Light = CreateEntity();
		SetEntityTag(m_Light, "SceneLight");
		AddComponent(m_Light, Transform{ });
		AddComponent(m_Light, PointLight{ 3.0f, 400.0f, sf::Color(219, 113, 114, 255) });
		AddNativeScript<GoToMouse>(m_Light);
		// place both lights onto the same entity
		AddComponent(m_Light, DirectionalLight{ 0.0f, -1.7f, 0.3f, sf::Color(200, 61, 19, 255) });
	}

private:
	Entity m_MainText = INVALID_ENTITY_ID;
	Entity m_Camera = INVALID_ENTITY_ID;
	Entity m_FadeInController = INVALID_ENTITY_ID;

	Entity m_ReplayButton = INVALID_ENTITY_ID;
	Entity m_MenuButton = INVALID_ENTITY_ID;

	Entity m_Light = INVALID_ENTITY_ID;
};
