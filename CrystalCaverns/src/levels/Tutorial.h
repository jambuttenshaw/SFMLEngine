#pragma once

#include <SFMLEngine.h>

#include "LevelFactory.h"
#include "../entities/Wolf.h"


using namespace SFMLEngine;

class Tutorial : public Scene
{
public:
	void Create() override
	{
		
		LevelFactory::CreateGround(this, "assets/tilemaps/tutorial/layer0.json", "assets/tilemaps/tutorial/layer0_collider.json");
		LevelFactory::CreateJumpThroughPlatforms(this, "assets/tilemaps/tutorial/layer1.json");
		LevelFactory::CreateLadders(this, "assets/tilemaps/tutorial/layer2.json");
		LevelFactory::CreateCrystals(this, "assets/tilemaps/tutorial/layer3.json");
		
		LevelFactory::CreateLevelEnd(this, { 2240, 736 });
		// LevelFactory::CreateLevelEnd(this, { 0, 0 });

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
		bool* loadedLevel1 = DataStore::RetrieveData<bool>("loadedLevel1");
		*loadedLevel1 = false;
	}
};

