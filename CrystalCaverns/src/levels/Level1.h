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
			{   864, 1888 },
			{ - 928, 1536 },
			{    64,  768 },
			{  1184,  992 },
			{  2080, 1120 },
			{  1888,  320 },
			{ -1632, 1216 }
		};

		for (auto& pos : healthPickupPositions)
			HealthPickup::Create(this, pos);


		// WOLVES

		std::vector<sf::Vector2f> wolfPositions{
			{   224,   512 },
			{  1088,   288 },
			{  1440,   448 },
			{   960,   864 },
			{  1760,   992 },
			{  1536,  1376 },
			{  1088,  1568 },
			{   800,  1920 },
			{   448,  1856 },
			{ - 544,  1600 },
			{ - 320,  1120 },
			{ - 1440, 1312 },
			{ - 1248,  704 },
			{      0,  160 }
		};

		for (auto& pos : wolfPositions)
			Wolf::Create(this, pos);


		std::vector<sf::Vector2f> wolfBlockers
		{
			{ - 640,  608 },
			{   832,  256 },
			{   608,  512 },
			{    32,  544 },
			{ -1280, 1216 },
			{  1920,  992 }
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

