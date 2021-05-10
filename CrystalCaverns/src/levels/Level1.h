#pragma once

#include <SFMLEngine.h>

// scripts
#include "../entities/Wolf.h"
#include "game/WolfController.h"

#include "../entities/HealthPickup.h"

#include "LevelFactory.h"


using namespace SFMLEngine;


/*
Level1 is the main level in Crystal Caverns


It is a scene in the game, so it must inherit from scene
*/
class Level1 : public Scene
{
public:
	void Create() override
	{
		// create all of the content of the level

		// create the standard components of the level
		// since level1 is so big, its collision geometry is generated offline and stored in json format
		// loading from json is MUCH faster than running the tilemap collider optimization algorithm
		LevelFactory::CreateGround(this, "assets/tilemaps/level1/layer0.json", "assets/tilemaps/level1/layer0_collider.json");
		LevelFactory::CreateJumpThroughPlatforms(this, "assets/tilemaps/level1/layer1.json");
		LevelFactory::CreateLadders(this, "assets/tilemaps/level1/layer2.json");
		LevelFactory::CreateCrystals(this, "assets/tilemaps/level1/layer3.json");
		LevelFactory::CreateFill(this, "assets/tilemaps/level1/layer4.json");

		// the level should have a trigger to signify its end
		LevelFactory::CreateLevelEnd(this, { 0, -300 });

		// the main level requires just 1 bit of tutorial text to instruct players to go back up the ladder
		LevelFactory::CreateTutorialText(this, 11, { 50, -100 });


		// HEALTH PICKUPS

		// a list of all the locations health pickups should be placed at
		std::vector<sf::Vector2f> healthPickupPositions{
			{   864, 1888 },
			{ - 928, 1536 },
			{    64,  768 },
			{  1184,  992 },
			{  2080, 1120 },
			{  1888,  320 },
			{ -1632, 1216 }
		};

		// create a health pick-up object for each location
		for (auto& pos : healthPickupPositions)
			HealthPickup::Create(this, pos);


		// WOLVES

		// specify where the wolves should be placed in the level
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

		// create a wolf entity for each position
		for (auto& pos : wolfPositions)
			Wolf::Create(this, pos);

		// wolf blockers stop the wolves from leaving areas they wouldnt be able to get back to
		// they are just to stop all the wolves eventually congregating in the same place
		std::vector<sf::Vector2f> wolfBlockers
		{
			{ - 640,  608 },
			{   832,  256 },
			{   608,  512 },
			{    32,  544 },
			{ -1280, 1216 },
			{  1920,  992 }
		};

		// create a wolf blocker gameobject for every position specified
		for (auto& pos : wolfBlockers)
			CreateWolfBlocker(pos);
	}


	void CreateWolfBlocker(const sf::Vector2f& pos)
	{
		// creates a wolf blocker

		// tag the entity so the wolves can identify it as a blocker
		Entity wolfBlocker = CreateEntity();
		SetEntityTag(wolfBlocker, "WolfBlocker");

		// place the blocker at the position specified
		AddComponent(wolfBlocker, Transform{ pos });

		// add a trigger collider to it
		// it should be the size of a whole tile
		BoxCollider collider{ { 32, 32 }, { 0, 0 } };
		collider.SetTrigger(true);
		AddComponent(wolfBlocker, collider);
		AddComponent(wolfBlocker, ColliderInfo{ ColliderType::Box });

		// I know that no further processing of this wolf blocker entity will be carried out
		// so I do not return the entity handle
	}

};

