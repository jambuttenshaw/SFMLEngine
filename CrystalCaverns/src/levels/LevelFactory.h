#pragma once


#include <SFMLEngine.h>

// scripts
#include "../entities/Wolf.h"
#include "game/WolfController.h"
#include "tutorial/TutorialText.h"


using namespace SFMLEngine;


class LevelFactory
{
public:
	// init
	static void Init();

	// factory functions for creating basic level components
	static Entity CreateGround(Scene* scene, const std::string& path, const std::string& colliderPath = "null");
	static Entity CreateJumpThroughPlatforms(Scene* scene, const std::string& path);
	static Entity CreateLadders(Scene* scene, const std::string& path);
	static Entity CreateCrystals(Scene* scene, const std::string& path);
	static Entity CreateLevelEnd(Scene* scene, const sf::Vector2f& position);
	static Entity CreateTutorialText(Scene* scene, int textID, const sf::Vector2f& position);
	static Entity CreateFill(Scene* scene, const std::string& path);

private:
	// the ID of the palette resource
	// it needs to be used across levels
	static ResourceID s_TilePaletteID;
};