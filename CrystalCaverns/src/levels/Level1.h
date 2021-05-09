#pragma once

#include <SFMLEngine.h>

// scripts
#include "../entities/Wolf.h"
#include "game/WolfController.h"

#include "../entities/HealthPickup.h"

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
		LevelFactory::CreateFill(this, "assets/tilemaps/level1/layer4.json");

		LevelFactory::CreateLevelEnd(this, { 0, -300 });

		LevelFactory::CreateTutorialText(this, 11, { 50, -100 });


		// HEALTH PICKUPS
		std::vector<sf::Vector2f> healthPickupPositions{
			{ - 704,  608 },
			{     0,  768 },
			{  1120,  992 },
			{  2272,  896 },
			{ -1504, 1088 }
		};

		for (auto& pos : healthPickupPositions)
			HealthPickup::Create(this, pos);


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
			{ -1184,  704 },
			{   160,  512 }
		};

		for (auto& pos : wolfPositions)
			Wolf::Create(this, pos);


		std::vector<sf::Vector2f> wolfBlockers
		{
			{ - 640,  608 },
			{   832,  256 },
			{   608,  512 },
			{    32,  544 }
		};

		for (auto& pos : wolfBlockers)
			CreateWolfBlocker(pos);
	}


	void CreateWolfBlocker(const sf::Vector2f& pos)
	{
		Entity wolfBlocker = CreateEntity();
		SetEntityTag(wolfBlocker, "WolfBlocker");
		AddComponent(wolfBlocker, Transform{ pos });

		BoxCollider collider{ { 32, 32 }, { 0, 0 } };
		collider.SetTrigger(true);
		AddComponent(wolfBlocker, collider);
		AddComponent(wolfBlocker, ColliderInfo{ ColliderType::Box });
	}

};

