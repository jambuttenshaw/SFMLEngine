#pragma once

#include <SFMLEngine.h>
#include "pauseMenu/ClosePauseMenu.h"
#include "pauseMenu/PauseMenuButton.h"


using namespace SFMLEngine;


/*
The pause menu has a background image with 3 buttons on it
One for resuming, one for going to menu and one for quitting the game
*/


class PauseMenu : public Scene
{
public:

	void Create() override
	{
		// create the background image
		// the pause menu is implemented as part of the HUD
		// therefore positions are specified in normalized coordinates and positioning and scaling is handled by the gui system
		Entity background = CreateEntity();
		AddComponent(background, GUITransform{ { 0.083f, 0.296f }, GUIElementType::Image });
		AddComponent(background, GUIImage{ Texture::Create("assets/textures/pauseMenu/background.png"), 100 });


		// buttons are also just images, but have a button script attached that handles the transitions

		// create a pause button and attach the pause button script
		Entity resumeButton = CreateEntity();
		AddComponent(resumeButton, GUITransform{ { 0.104f, 0.333f }, GUIElementType::Image });
		AddComponent(resumeButton, GUIImage{ Texture::Create("assets/textures/pauseMenu/resume.png"), 101 });
		AddNativeScript<PResumeButton>(resumeButton);

		// create a menu button and attach the menu button script
		Entity menuButton = CreateEntity();
		AddComponent(menuButton, GUITransform{ { 0.104f, 0.563f }, GUIElementType::Image });
		AddComponent(menuButton, GUIImage{ Texture::Create("assets/textures/pauseMenu/menu.png"), 101 });
		AddNativeScript<PMenuButton>(menuButton);

		// create a quit button and attach the quit button script
		Entity quitButton = CreateEntity();
		AddComponent(quitButton, GUITransform{ { 0.104f, 0.793f }, GUIElementType::Image });
		AddComponent(quitButton, GUIImage{ Texture::Create("assets/textures/pauseMenu/quit.png"), 101 });
		AddNativeScript<PQuitButton>(quitButton);


		// create an entity with a script attached that is dedicated to closing the pause menu
		Entity closePauseMenu = CreateEntity();
		SetEntityTag(closePauseMenu, "ClosePauseMenu");
		auto& script = AddNativeScript<ClosePauseMenu>(closePauseMenu);
		script.SetScene(this);
	}
};
