#pragma once

#include <SFMLEngine.h>

#include "LevelFactory.h"
#include "../entities/Wolf.h"
#include "tutorial/TutorialText.h"


using namespace SFMLEngine;

class Tutorial : public Scene
{
public:
	void Create() override
	{
		
		m_Ground = LevelFactory::CreateGround(this, "assets/tilemaps/tutorial/layer0.json", "assets/tilemaps/tutorial/layer0_collider.json");
		m_JumpThroughPlatforms = LevelFactory::CreateJumpThroughPlatforms(this, "assets/tilemaps/tutorial/layer1.json");
		m_Ladders = LevelFactory::CreateLadders(this, "assets/tilemaps/tutorial/layer2.json");
		m_Crystals = LevelFactory::CreateCrystals(this, "assets/tilemaps/tutorial/layer3.json");
		
		m_LevelEnd = LevelFactory::CreateLevelEnd(this, { 1696, 32 });

		// m_Wolf = Wolf::Create(this, { 224, 64 });

		CreateTutorialText(1, { 100, 0 });


		// flag that we are entering the tutorial
		// level 1 should follow
		// so flag that level 1 has not been loaded
		bool* loadedLevel1 = DataStore::RetrieveData<bool>("loadedLevel1");
		*loadedLevel1 = false;
	}


	void CreateTutorialText(int textID, const sf::Vector2f& position)
	{
		Entity newText = CreateEntity();
		auto& script = AddNativeScript<TutorialText>(newText);
		script.Init(textID, position);
	}

private:
	Entity m_Ground = INVALID_ENTITY_ID;
	Entity m_JumpThroughPlatforms = INVALID_ENTITY_ID;

	Entity m_Ladders = INVALID_ENTITY_ID;
	Entity m_Crystals = INVALID_ENTITY_ID;

	Entity m_LevelEnd = INVALID_ENTITY_ID;
	Entity m_Wolf = INVALID_ENTITY_ID;
};

