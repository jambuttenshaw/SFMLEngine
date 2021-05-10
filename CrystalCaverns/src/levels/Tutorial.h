#pragma once

#include <SFMLEngine.h>

#include "LevelFactory.h"
#include "../entities/Wolf.h"


using namespace SFMLEngine;

/*
The tutorial tells the player how to play the game
what controls to use, what the objectives are etc
*/
class Tutorial : public Scene
{
public:
	void Create() override
	{
		// create the basic components of the level
		// the collision geometry for the ground has been created in advance for faster loading times
		LevelFactory::CreateGround(this, "assets/tilemaps/tutorial/layer0.json", "assets/tilemaps/tutorial/layer0_collider.json");
		LevelFactory::CreateJumpThroughPlatforms(this, "assets/tilemaps/tutorial/layer1.json");
		LevelFactory::CreateLadders(this, "assets/tilemaps/tutorial/layer2.json");
		LevelFactory::CreateCrystals(this, "assets/tilemaps/tutorial/layer3.json");
		LevelFactory::CreateFill(this, "assets/tilemaps/tutorial/layer4.json");
		
		// place the level end for  the tutorial so the player knows when to switch levels
		LevelFactory::CreateLevelEnd(this, { 2240, 736 });

		// add in all of the tutorial text to describe how to play
		// the increasing number parameter specifies which text to display
		LevelFactory::CreateTutorialText(this, 1, { 62, 44 });
		LevelFactory::CreateTutorialText(this, 2, { 240, -20 });
		LevelFactory::CreateTutorialText(this, 3, { 480, -165 });
		LevelFactory::CreateTutorialText(this, 4, { 540, 65 });
		LevelFactory::CreateTutorialText(this, 5, { 950, 10 });
		LevelFactory::CreateTutorialText(this, 6, { 1140, -60 });
		LevelFactory::CreateTutorialText(this, 7, { 1445, -20 });
		LevelFactory::CreateTutorialText(this, 8, { 1645, 510 });
		LevelFactory::CreateTutorialText(this, 9, { 1900, 560 });
		LevelFactory::CreateTutorialText(this, 10, { 2155, 555 });
		

		// flag that we are entering the tutorial
		// level 1 should follow
		// so flag that level 1 has not been loaded
		// this is used to decide between loading either the win screen or level1, depending if we have already played the tutorial or not
		bool* loadedLevel1 = DataStore::RetrieveData<bool>("loadedLevel1");
		*loadedLevel1 = false;
	}
};

