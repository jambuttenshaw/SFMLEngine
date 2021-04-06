#include <SFMLEngine.h>
#include <SFMLEngine/EntryPoint.h> // mark this file as the entry point

#include <SFML/Graphics.hpp>

#include "MainMenu.h"
#include "MainScene.h"


using namespace SFMLEngine;

class SandboxApp : public Application
{
public:
	SandboxApp()
		: Application("Sandbox", sf::Vector2i(1200, 675))
	{
		SetClearColor(sf::Color::Black);
		SetVSync(true);

		LoadScene<MainMenu>(LoadSceneMode::Single);
	}
};

Application* SFMLEngine::CreateApplication()
{
	return new SandboxApp;
}
