#pragma once

#include <SFMLEngine.h>

#include "menu/PlayButton.h"
#include "menu/QuitButton.h"

#include "menu/GoToMouse.h"
#include "menu/MenuCameraController.h"

using namespace SFMLEngine;


class MainMenu : public Scene
{
public:

	void Create() override
	{

		

		{
			m_PlayButton = CreateEntity();

			AddComponent(m_PlayButton, Transform{ { -96, -48 } });
			AddComponent(m_PlayButton, SpriteRenderer{
					Texture::Create("assets/textures/playButton.png"),
					Material::Create("Lit"),
					0,
					Texture::Create("assets/textures/playButton_n.png")
				});
			
			AddComponent(m_PlayButton, BoxCollider{ {192, 96}, {0, 0} });
			AddComponent(m_PlayButton, ColliderInfo{ ColliderType::Box });

			AddNativeScript<PlayButton>(m_PlayButton);
		}


		
		{
			m_QuitButton = CreateEntity();

			AddComponent(m_QuitButton, Transform{ { -96, 68 } });
			AddComponent(m_QuitButton, SpriteRenderer{
					Texture::Create("assets/textures/quitButton.png"),
					Material::Create("Lit"),
					0,
					Texture::Create("assets/textures/quitButton_n.png")
				});

			AddComponent(m_QuitButton, BoxCollider{ {192, 96}, {0, 0} });
			AddComponent(m_QuitButton, ColliderInfo{ ColliderType::Box });

			AddNativeScript<QuitButton>(m_QuitButton);
		}
		


		m_Light = CreateEntity();
		SetEntityTag(m_Light, "SceneLight");
		AddComponent(m_Light, Transform{ });
		AddComponent(m_Light, PointLight{ 2.5f, 300.0f, sf::Color(219, 113, 114, 255) });
		AddNativeScript<GoToMouse>(m_Light);

		m_Camera = CreateEntity();

		AddComponent(m_Camera, Transform{ });
		AddComponent(m_Camera, Camera{ });
		AddNativeScript<MenuCameraController>(m_Camera);
	}

private:
	Entity m_PlayButton = INVALID_ENTITY_ID;
	Entity m_QuitButton = INVALID_ENTITY_ID;
	Entity m_Camera = INVALID_ENTITY_ID;
	Entity m_Light = INVALID_ENTITY_ID;
};
