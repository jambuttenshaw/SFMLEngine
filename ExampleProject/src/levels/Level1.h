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

		m_Ground = LevelFactory::CreateGround(this, "assets/tilemaps/level1/layer0.json");
		m_JumpThroughPlatforms = LevelFactory::CreateJumpThroughPlatforms(this, "assets/tilemaps/level1/layer1.json");
		// m_Ladders = LevelFactory::CreateLadders(this, "assets/tilemaps/level1/layer2.json");
		// m_Crystals = LevelFactory::CreateCrystals(this, "assets/tilemaps/level1/layer3.json");

		m_Enemy = Wolf::Create(this, { -128, -64 });
	}

private:
	Entity m_Ground = INVALID_ENTITY_ID;
	Entity m_JumpThroughPlatforms = INVALID_ENTITY_ID;

	Entity m_Ladders = INVALID_ENTITY_ID;
	Entity m_Crystals = INVALID_ENTITY_ID;

	Entity m_Enemy = INVALID_ENTITY_ID;
};

