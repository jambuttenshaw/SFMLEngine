#pragma once

#include <SFMLEngine.h>

// scripts
#include "../entities/Wolf.h"
#include "game/WolfController.h"

#include "LevelFactory.h"


using namespace SFMLEngine;

class Level1 : public Scene
{
public:
	void Create() override
	{

		LevelFactory::CreateGround(this, "assets/tilemaps/level1/layer0.json", "assets/tilemaps/level1/layer0_collider.json");
		LevelFactory::CreateJumpThroughPlatforms(this, "assets/tilemaps/level1/layer1.json");
		LevelFactory::CreateLadders(this, "assets/tilemaps/level1/layer2.json");
		LevelFactory::CreateCrystals(this, "assets/tilemaps/level1/layer3.json");

		LevelFactory::CreateLevelEnd(this, { 0, -300 });

		LevelFactory::CreateTutorialText(this, 11, { 50, -100 });



		// WOLVES

		std::vector<sf::Vector2f> wolfPositions{
			{  1088,  288 },
			{  1504,  448 },
			{   992,  864 },
			{  2016,  992 },
			{  1536,  960 },
			{ - 224, 1120 },
			{ - 864, 1088 },
			{ -1184, 1152 },
			{ -1504, 1312 },
			{ -1184,  704 }
		};

		for (auto& pos : wolfPositions)
			Wolf::Create(this, pos);
	}
};

