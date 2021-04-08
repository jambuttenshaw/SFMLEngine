#pragma once


#include <SFMLEngine.h>

// scripts
#include "../entities/Wolf.h"
#include "game/WolfController.h"


using namespace SFMLEngine;


class LevelFactory
{
public:
	static void Init()
	{
		s_TilePaletteID = TilePalette::LoadFromFile("assets/palettes/terrainPalette.json");
	}


	static Entity CreateGround(Scene* scene, const std::string& path)
	{
		Entity ground = scene->CreateEntity();
		scene->SetEntityLayer(ground, "Ground");

		scene->AddComponent(ground, Transform{ });

		// add a tilemap

		scene->AddComponent(ground, Tilemap{ s_TilePaletteID, path });
		scene->AddComponent(ground, TilemapRenderer{ Material::Create("LitTilemap"), 1 });


		// this object should be solid
		// add a collider
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
		scene->AddComponent(jumpThroughPlatforms, TilemapRenderer{ Material::Create("LitTilemap"), 1 });

		// this object should be solid
		// add a collider
		TilemapCollider collider{ TilemapCollider::OptimizationLevel::High };
		collider.IsTrigger = true;
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
		scene->AddComponent(ladders, TilemapRenderer{ Material::Create("LitTilemap"), 1 });

		// this object should have a trigger collider so that the player knows when its collided with a ladder
		TilemapCollider collider{ TilemapCollider::OptimizationLevel::High };
		collider.IsTrigger = true;
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
		scene->AddComponent(crystals, TilemapRenderer{ Material::Create("LitTilemap"), 1 });

		// we need every crystal to have a unique collider so we do not optimize at all
		TilemapCollider collider{ TilemapCollider::OptimizationLevel::None };
		collider.IsTrigger = true;
		scene->AddComponent(crystals, collider);
		scene->AddComponent(crystals, ColliderInfo{ ColliderType::Tilemap });

		return crystals;
	}

private:
	static ResourceID s_TilePaletteID;
};