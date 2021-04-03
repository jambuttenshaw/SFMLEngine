#pragma once

#include <SFMLEngine.h>

#include "menu/PlayButton.h"


using namespace SFMLEngine;


class MainMenu : public Scene
{
public:

	void Create() override
	{
		{
			m_PlayButton = CreateEntity();

			AddComponent(m_PlayButton, Transform{ { -90, -40 } });
			AddComponent(m_PlayButton, SpriteRenderer{
					Texture::Create("assets/textures/playButton.png"),
					Material::Create("Basic"),
					0
				});
			
			AddComponent(m_PlayButton, BoxCollider{ {180, 80}, {0, 0} });
			AddComponent(m_PlayButton, ColliderInfo{ ColliderType::Box });

			AddNativeScript<PlayButton>(m_PlayButton);
		}



		m_Camera = CreateEntity();

		AddComponent(m_Camera, Transform{ });
		AddComponent(m_Camera, Camera{ });
	}

private:
	Entity m_PlayButton = INVALID_ENTITY_ID;
	Entity m_Camera = INVALID_ENTITY_ID;
};
