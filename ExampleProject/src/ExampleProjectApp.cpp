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

		
		// set up the data that should exist between scenes
		DataStore::CreatePersistentData("loadedLevel1", false);
		DataStore::CreatePersistentData("playerData", PlayerData{ });


		LevelFactory::Init();

		LoadScene<SplashScreen>(LoadSceneMode::Single);
	}

	~SandboxApp()
	{
		// contents of the data store should be destroyed manually
		// so they can be cast to the correct type before deletion
		DataStore::DeleteData<bool>("loadedLevel1");
		DataStore::DeleteData<PlayerData>("playerData");
	}
};

Application* SFMLEngine::CreateApplication()
{
	return new SandboxApp;
}
