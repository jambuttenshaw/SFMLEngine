#pragma once

#include <SFMLEngine.h>
#include "pauseMenu/ClosePauseMenu.h"
#include "pauseMenu/PauseMenuButton.h"


using namespace SFMLEngine;


class PauseMenu : public Scene
{
public:

	void Create() override
	{
		m_Background = CreateEntity();
		AddComponent(m_Background, GUITransform{ { 0.083f, 0.296f }, GUIElementType::Image });
		AddComponent(m_Background, GUIImage{ Texture::Create("assets/textures/pauseMenu/background.png"), 100 });



		m_ResumeButton = CreateEntity();
		AddComponent(m_ResumeButton, GUITransform{ { 0.104f, 0.333f }, GUIElementType::Image });
		AddComponent(m_ResumeButton, GUIImage{ Texture::Create("assets/textures/pauseMenu/resume.png"), 101 });
		AddNativeScript<PResumeButton>(m_ResumeButton);

		m_MenuButton = CreateEntity();
		AddComponent(m_MenuButton, GUITransform{ { 0.104f, 0.563f }, GUIElementType::Image });
		AddComponent(m_MenuButton, GUIImage{ Texture::Create("assets/textures/pauseMenu/menu.png"), 101 });
		AddNativeScript<PMenuButton>(m_MenuButton);

		m_QuitButton = CreateEntity();
		AddComponent(m_QuitButton, GUITransform{ { 0.104f, 0.793f }, GUIElementType::Image });
		AddComponent(m_QuitButton, GUIImage{ Texture::Create("assets/textures/pauseMenu/quit.png"), 101 });
		AddNativeScript<PQuitButton>(m_QuitButton);



		m_ClosePauseMenu = CreateEntity();
		SetEntityTag(m_ClosePauseMenu, "ClosePauseMenu");
		auto& script = AddNativeScript<ClosePauseMenu>(m_ClosePauseMenu);
		script.SetScene(this);
	}


private:
	Entity m_Background = INVALID_ENTITY_ID;

	Entity m_ResumeButton = INVALID_ENTITY_ID;
	Entity m_MenuButton = INVALID_ENTITY_ID;
	Entity m_QuitButton = INVALID_ENTITY_ID;

	Entity m_ClosePauseMenu = INVALID_ENTITY_ID;
};
