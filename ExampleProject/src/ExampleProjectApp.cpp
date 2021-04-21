#include <SFMLEngine.h>
#include <SFMLEngine/EntryPoint.h> // mark this file as the entry point

#include <SFML/Graphics.hpp>

#include "SplashScreen.h"
#include "MainMenu.h"
#include "MainScene.h"

#include "levels/LevelFactory.h"
#include "levels/AllLevels.h"

#include "DeathScreen.h"


using namespace SFMLEngine;

class SandboxApp : public Application
{
public:
	SandboxApp()
		: Application("Sandbox", sf::Vector2i(1200, 675))
	{
		SetClearColor(sf::Color::Black);
		SetVSync(true);

		LevelFactory::Init();

		LoadScene<SplashScreen>(LoadSceneMode::Single);
	}
};

Application* SFMLEngine::CreateApplication()
{
	return new SandboxApp;
}
