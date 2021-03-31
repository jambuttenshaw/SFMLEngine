#pragma once

#include <SFMLEngine.h>

#include "MainMenuManager.h"


using namespace SFMLEngine;


class MainMenu : public Scene
{
public:

	void Create() override
	{
		m_Entity = CreateEntity();

		AddComponent(m_Entity, Transform{ { 0, 0 }, 0, { 4.0f, 4.0f } });
		AddComponent(m_Entity, SpriteRenderer{
				Texture::Create("assets/textures/wolf.png"),
				Material::Create("Basic"),
				0 });



		m_MenuManager = CreateEntity();
		AddNativeScript<MainMenuManager>(m_MenuManager);


		

		m_Camera = CreateEntity();

		AddComponent(m_Camera, Transform{ });
		AddComponent(m_Camera, Camera{ });
	}

private:
	Entity m_Entity = INVALID_ENTITY_ID;
	Entity m_MenuManager = INVALID_ENTITY_ID;
	Entity m_Camera = INVALID_ENTITY_ID;
};
