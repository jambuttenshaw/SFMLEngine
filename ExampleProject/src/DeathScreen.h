#pragma once

#include <SFMLEngine.h>
#include "menu/FadeInController.h"
#include "deathScreen/DeathScreenButtons.h"

using namespace SFMLEngine;


class DeathScreen : public Scene
{
public:

	void Create() override
	{
		{
			m_ReplayButton = CreateEntity();

			AddComponent(m_ReplayButton, Transform{ { -200, 300 } });
			AddComponent(m_ReplayButton, SpriteRenderer{
					Texture::Create("assets/textures/buttons/replay_temp.png"),
					Material::Create("Basic"),
					0
				});

			AddComponent(m_ReplayButton, BoxCollider{ {128, 64}, {0, 0} });
			AddComponent(m_ReplayButton, ColliderInfo{ ColliderType::Box });

			AddNativeScript<ReplayButton>(m_ReplayButton);
		}
		{
			m_MenuButton = CreateEntity();

			AddComponent(m_MenuButton, Transform{ { 200, 300 } });
			AddComponent(m_MenuButton, SpriteRenderer{
					Texture::Create("assets/textures/buttons/menu_temp.png"),
					Material::Create("Basic"),
					0
				});

			AddComponent(m_MenuButton, BoxCollider{ {128, 64}, {0, 0} });
			AddComponent(m_MenuButton, ColliderInfo{ ColliderType::Box });

			AddNativeScript<ExitToMenuButton>(m_MenuButton);
		}


		m_MainText = CreateEntity();
		AddComponent(m_MainText, Transform{ { -140, -52 } });
		AddComponent(m_MainText, SpriteRenderer{
			Texture::Create("assets/textures/deathScreen_lit.png"),
			Material::Create("Basic"),
			0
			});


		m_Camera = CreateEntity();
		SetEntityTag(m_Camera, "Camera");
		AddComponent(m_Camera, Transform{ });
		AddComponent(m_Camera, Camera{ });


		m_FadeInController = CreateEntity();
		AddNativeScript<FadeInController>(m_FadeInController);
	}

private:
	Entity m_MainText = INVALID_ENTITY_ID;
	Entity m_Camera = INVALID_ENTITY_ID;
	Entity m_FadeInController = INVALID_ENTITY_ID;

	Entity m_ReplayButton = INVALID_ENTITY_ID;
	Entity m_MenuButton = INVALID_ENTITY_ID;
};
