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

class CrystalCavernsApp : public Application
{
public:
	CrystalCavernsApp()
		// give the application a title and a default window size
		: Application("Crystal Caverns", sf::Vector2i(1200, 675))
	{
		// set the clear colour
		// black blends in best if there are any gaps
		SetClearColor(sf::Color::Black);
		// use vsync to limit the framerate
		SetVSync(true);
		
		// set up the data that should exist between scenes
		// this allocates a bool and an instance of player data
		// and associates a string with these allocations so they can be retrieved
		// and utilized later
		DataStore::CreatePersistentData("loadedLevel1", false);
		DataStore::CreatePersistentData("playerData", PlayerData{ });
		

		// initialise the level factory to create level components
		LevelFactory::Init();


		// load the font
		// it is used by multiple scenes so we just load it in advance of any of them
		// to have it available to all of them
		FontLibrary::LoadNewFont("arcade", "assets/fonts/ARCADECLASSIC.TTF");



		// button click is used by so many different entities that its easier just to make sure it exists in memory
		// right from the start
		AudioSystem::LoadSound("buttonClick", "assets/audio/click.ogg");
		AudioSystem::SetRelativeToListener("buttonClick", true);

		// load the music
		AudioSystem::LoadMusic("assets/audio/music.ogg");
		AudioSystem::SetMusicLooping(true);
		AudioSystem::SetMusicVolume(30);


		// then finally load the splash screen
		// the splash screen handles transition to the menu, which handles transition into the game etc...
		LoadScene<SplashScreen>(LoadSceneMode::Single);
	}

	~CrystalCavernsApp()
	{
		// contents of the data store should be destroyed manually
		// so they can be cast to the correct type before deletion
		DataStore::DeleteData<bool>("loadedLevel1");
		DataStore::DeleteData<PlayerData>("playerData");
	}
};


// CreateApplication is the handle into which the application can register itself with the engine
// using this function the engine knows exactly what type of application it is to create and run
Application* SFMLEngine::CreateApplication()
{
	return new CrystalCavernsApp;
}
