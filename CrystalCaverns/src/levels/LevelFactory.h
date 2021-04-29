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
	static void Init()
	{
		s_TilePaletteID = TilePalette::LoadFromFile("assets/palettes/terrainPalette.json");
	}


	static Entity CreateGround(Scene* scene, const std::string& path, const std::string& colliderPath = "null")
	{
		Entity ground = scene->CreateEntity();
		scene->SetEntityLayer(ground, "Ground");
		scene->SetEntityTag(ground, "Ground");

		scene->AddComponent(ground, Transform{ });

		// add a tilemap

		scene->AddComponent(ground, Tilemap{ s_TilePaletteID, path });
		scene->AddComponent(ground, TilemapRenderer{ Material::Create("LitTilemap"), 0 });


		// this object should be solid
		// add a collider
		if (colliderPath != "null")
			scene->AddComponent(ground, TilemapCollider{ colliderPath });
		else
			scene->AddComponent(ground, TilemapCollider{ TilemapCollider::OptimizationLevel::High });
		scene->AddComponent(ground, ColliderInfo{ ColliderType::Tilemap });

		return ground;
	}

	static Entity CreateJumpThroughPlatforms(Scene* scene, const std::string& path)
	{
		Entity jumpThroughPlatforms = scene->CreateEntity();
		scene->SetEntityLayer(jumpThroughPlatforms, "JumpThrough");

		scene->AddComponent(jumpThroughPlatforms, Transform{ });

		scene->AddComponent(jumpThroughPlatforms, Tilemap{ s_TilePaletteID, path });
		scene->AddComponent(jumpThroughPlatforms, TilemapRenderer{ Material::Create("LitTilemap"), 0 });

		// this object should be solid
		// add a collider
		TilemapCollider collider{ TilemapCollider::OptimizationLevel::High };
		collider.SetTrigger(true);
		scene->AddComponent(jumpThroughPlatforms, collider);
		scene->AddComponent(jumpThroughPlatforms, ColliderInfo{ ColliderType::Tilemap });

		return jumpThroughPlatforms;
	}

	static Entity CreateLadders(Scene* scene, const std::string& path)
	{
		Entity ladders = scene->CreateEntity();
		scene->SetEntityTag(ladders, "Ladder");
		scene->SetEntityLayer(ladders, "Ground");

		scene->AddComponent(ladders, Transform{ });

		scene->AddComponent(ladders, Tilemap{ s_TilePaletteID, path });
		scene->AddComponent(ladders, TilemapRenderer{ Material::Create("LitTilemap"), 0 });

		// this object should have a trigger collider so that the player knows when its collided with a ladder
		TilemapCollider collider{ TilemapCollider::OptimizationLevel::High };
		collider.SetTrigger(true);
		scene->AddComponent(ladders, collider);
		scene->AddComponent(ladders, ColliderInfo{ ColliderType::Tilemap });

		return ladders;
	}

	static Entity CreateCrystals(Scene* scene, const std::string& path)
	{
		Entity crystals = scene->CreateEntity();
		scene->SetEntityTag(crystals, "Crystals");

		scene->AddComponent(crystals, Transform{ });

		scene->AddComponent(crystals, Tilemap{ s_TilePaletteID, path });
		scene->AddComponent(crystals, TilemapRenderer{ Material::Create("LitTilemap"), 0 });

		// we need every crystal to have a unique collider so we do not optimize at all
		TilemapCollider collider{ TilemapCollider::OptimizationLevel::None };
		collider.SetTrigger(true);
		scene->AddComponent(crystals, collider);
		scene->AddComponent(crystals, ColliderInfo{ ColliderType::Tilemap });

		return crystals;
	}


	static Entity CreateLevelEnd(Scene* scene, const sf::Vector2f& position)
	{
		Entity levelEnd = scene->CreateEntity();
		scene->SetEntityTag(levelEnd, "LevelEnd");

		scene->AddComponent(levelEnd, Transform{ position });
		BoxCollider collider{ { 32, 32 }, { 0, 0 } };
		collider.SetTrigger(true);
		scene->AddComponent(levelEnd, collider);
		scene->AddComponent(levelEnd, ColliderInfo{ ColliderType::Box });

		return levelEnd;
	}


	static Entity CreateTutorialText(Scene* scene, int textID, const sf::Vector2f& position)
	{
		Entity newText = scene->CreateEntity();
		auto& script = scene->AddNativeScript<TutorialText>(newText);
		script.Init(textID, position);

		return newText;
	}


private:
	static ResourceID s_TilePaletteID;
};