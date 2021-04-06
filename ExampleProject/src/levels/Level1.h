#pragma once

#include <SFMLEngine.h>

// scripts
#include "../entities/Wolf.h"
#include "game/WolfController.h"


using namespace SFMLEngine;

class Level1 : public Scene
{
public:
	void Create() override
	{
		ResourceID tilePaletteID = TilePalette::LoadFromFile("assets/palettes/terrainPalette.json");
		TilePalette* tilePalette = ResourceManager::GetResourceHandle<TilePalette>(tilePaletteID);

		{
			m_Ground = CreateEntity();
			SetEntityLayer(m_Ground, "Ground");

			AddComponent(m_Ground, Transform{ });

			// add a tilemap

			AddComponent(m_Ground, Tilemap{ tilePaletteID, "assets/tilemaps/level1/layer0.json" });
			AddComponent(m_Ground, TilemapRenderer{ Material::Create("LitTilemap"), 1 });


			// this object should be solid
			// add a collider
			AddComponent(m_Ground, TilemapCollider{ TilemapCollider::OptimizationLevel::High });
			AddComponent(m_Ground, ColliderInfo{ ColliderType::Tilemap });
		}


		{
			m_JumpThroughPlatforms = CreateEntity();
			SetEntityLayer(m_JumpThroughPlatforms, "JumpThrough");

			AddComponent(m_JumpThroughPlatforms, Transform{ });

			AddComponent(m_JumpThroughPlatforms, Tilemap{ tilePaletteID, "assets/tilemaps/level1/layer1.json" });
			AddComponent(m_JumpThroughPlatforms, TilemapRenderer{ Material::Create("LitTilemap"), 1 });

			// this object should be solid
			// add a collider
			TilemapCollider collider{ TilemapCollider::OptimizationLevel::High };
			collider.IsTrigger = true;
			AddComponent(m_JumpThroughPlatforms, collider);
			AddComponent(m_JumpThroughPlatforms, ColliderInfo{ ColliderType::Tilemap });
		}



		{
			m_Ladders = CreateEntity();
			SetEntityTag(m_Ladders, "Ladder");
			SetEntityLayer(m_Ladders, "Ground");

			AddComponent(m_Ladders, Transform{ });

			AddComponent(m_Ladders, Tilemap{ tilePaletteID, "assets/tilemaps/level1/layer2.json" });
			AddComponent(m_Ladders, TilemapRenderer{ Material::Create("LitTilemap"), 1 });

			// this object should have a trigger collider so that the player knows when its collided with a ladder
			TilemapCollider collider{ TilemapCollider::OptimizationLevel::High };
			collider.IsTrigger = true;
			AddComponent(m_Ladders, collider);
			AddComponent(m_Ladders, ColliderInfo{ ColliderType::Tilemap });
		}

		{
			m_Crystals = CreateEntity();
			SetEntityTag(m_Crystals, "Crystals");

			AddComponent(m_Crystals, Transform{ });

			AddComponent(m_Crystals, Tilemap{ tilePaletteID, "assets/tilemaps/level1/layer3.json" });
			AddComponent(m_Crystals, TilemapRenderer{ Material::Create("LitTilemap"), 1 });

			// we need every crystal to have a unique collider so we do not optimize at all
			TilemapCollider collider{ TilemapCollider::OptimizationLevel::None };
			collider.IsTrigger = true;
			AddComponent(m_Crystals, collider);
			AddComponent(m_Crystals, ColliderInfo{ ColliderType::Tilemap });
		}


		m_Enemy = Wolf::Create(this, { -128, -64 });
	}

private:
	Entity m_Ground = INVALID_ENTITY_ID;
	Entity m_JumpThroughPlatforms = INVALID_ENTITY_ID;

	Entity m_Ladders = INVALID_ENTITY_ID;
	Entity m_Crystals = INVALID_ENTITY_ID;

	Entity m_Enemy = INVALID_ENTITY_ID;
};

