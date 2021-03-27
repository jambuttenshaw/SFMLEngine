#include <SFMLEngine.h>
#include <SFMLEngine/EntryPoint.h> // mark this file as the entry point

#include <SFML/Graphics.hpp>

// scripts
#include "PlayerController.h"
#include "CrystalCollector.h"

#include "EnemyController.h"

#include "SmoothFollowPlayer.h"


using namespace SFMLEngine;


class MainScene : public Scene
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
			AddComponent(m_Ground, TilemapCollider { TilemapCollider::OptimizationLevel::High });
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
		

		{
			// create the player
			// its name, tag and layer should all be Player
			m_Player = CreateEntity("Player", "Player", "Player");

			// give the entity a transform
			AddComponent(m_Player, Transform{ sf::Vector2f(0, -200) });

			// add a rigidbody so this entity is affected by physics
			AddComponent(m_Player, Rigidbody{ });
			AddComponent(m_Player, BoxCollider{ sf::Vector2f(17, 48), sf::Vector2f(8, 16) });
			AddComponent(m_Player, ColliderInfo{ ColliderType::Box });

			// add the sprite renderer component
			AddComponent(m_Player, SpriteRenderer{
				Texture::Create("assets/textures/characterSheet.png"),
				Material::Create("Lit"),
				1,
				Texture::Create("assets/textures/characterSheetNormals.png") });


			// create the animations and animator component
			Animation idle{ "idle", {
				{0,   0, 32, 64},
				{34,  0, 32, 64},
				{66,  0, 32, 64},
				{96,  0, 32, 64},
				{129, 0, 32, 64},
				{161, 0, 32, 64} },
				0.1f
			};
			Animation run{ "run", {
				{1,   128, 32, 64},
				{33,  128, 32, 64},
				{64,  128, 32, 64},
				{96,  128, 32, 64},
				{128, 128, 32, 64},
				{160, 128, 32, 64} },
				0.1f
			};
			Animation jump{ "jump", {
				{0,  320, 32, 64},
				{32, 320, 32, 64},
				{64, 320, 32, 64} },
				0.1f
			}; jump.Looping = false;
			Animation punch{ "punch", {
				{0,  832, 32, 64},
				{32, 832, 35, 64, -3, 0},
				{67, 832, 32, 64} },
				0.1f
			}; punch.Looping = false;
			Animation climb{ "climb", {
				{0, 510, 32, 64}, 
				{29, 510, 32, 64}, 
				{63, 510, 32, 64}, 
				{93, 510, 32, 64}, 
				{126, 510, 32, 64} },
				0.15f
			};
			Animation idleCrawl{ "idleCrawl", {
				{0, 288, 64, 32} },
				0.1f
			}; idleCrawl.Looping = false;
			Animation crawl{ "crawl", {
				{0, 288, 64, 32},
				{61, 288, 64, 32},
				{126, 288, 64, 32},
				{192, 288, 64, 32},
				{253, 288, 64, 32},
				{317, 288, 64, 32},
				{384, 288, 64, 32} },
				0.1f
			};

			Animator animator;
			animator.AddAnimation(idle);
			animator.AddAnimation(run);
			animator.AddAnimation(jump);
			animator.AddAnimation(punch);
			animator.AddAnimation(climb);
			animator.AddAnimation(idleCrawl);
			animator.AddAnimation(crawl);
			animator.SetCurrentAnimation("idle");

			AddComponent(m_Player, animator);
			

			AddNativeScript<PlayerController>(m_Player);

			auto& script = AddNativeScript<CrystalCollector>(m_Player);
			script.SetCrystalMap(m_Crystals);
		}

		{
			m_Light = CreateEntity();

			// light is a child transform of physics entity
			AddComponent(m_Light, Transform{ sf::Vector2f(16, 32), & GetComponent<Transform>(m_Player) });
			AddComponent(m_Light, PointLight{ 3.0f, 200.0f, sf::Color(219, 113, 114, 255) });
		}

		{
			m_Camera = CreateEntity();

			AddComponent(m_Camera, Transform{ });
			
			Camera cam{ };
			cam.Zoom = 0.75f;
			AddComponent(m_Camera, cam);

			AddNativeScript<SmoothFollowPlayer>(m_Camera);
		}
		

		{
			m_Enemy = CreateEntity();
			SetEntityLayer(m_Enemy, "Enemies");

			AddComponent(m_Enemy, Transform{ { -416, -64 } });

			// add the sprite renderer component
			AddComponent(m_Enemy, SpriteRenderer{
				Texture::Create("assets/textures/wolfSheet.png"),
				Material::Create("Lit"),
				2,
				Texture::Create("assets/textures/wolfSheet_n.png") });

			AddComponent(m_Enemy, Rigidbody{ });
			AddComponent(m_Enemy, BoxCollider{ sf::Vector2f(64, 29), sf::Vector2f(0, 3) });
			AddComponent(m_Enemy, ColliderInfo{ ColliderType::Box });

			AddNativeScript<EnemyController>(m_Enemy);

			Animation idle{ "idle", {
				{ 0, 0, 64, 32 } },
				0.1f
			}; idle.Looping = false;

			Animation sleep{ "sleep", {
				{ 0,   0, 64, 32 },
				{ 64,  0, 64, 32 },
				{ 128, 0, 64, 32 },
				{ 192, 0, 64, 32 } },
				0.1f
			}; sleep.Looping = false;

			Animation wake{ "wake", {
				{ 192, 0, 64, 32 },
				{ 128, 0, 64, 32 },
				{ 64,  0, 64, 32 },
				{ 0,   0, 64, 32 } },
				0.05f
			}; wake.Looping = false;

			Animation alert{ "alert", {
				{ 0,   64, 64, 32,  0,  0 },
				{ 64,  59, 64, 37,  0, -5 },
				{ 128, 55, 64, 41,  0, -9 },
				{ 192, 55, 64, 41,  0, -9 } },
				0.1f
			}; alert.Looping = false;

			Animation walk{ "walk", {
				{ 0,   96, 64, 32 },
				{ 64,  96, 64, 32 },
				{ 128, 96, 64, 32 },
				{ 192, 96, 64, 32 },
				{ 256, 96, 64, 32 } },
				0.1f
			};

			Animation run{ "run", {
				{ 0,   128, 64, 32 },
				{ 64,  128, 64, 32 },
				{ 128, 128, 64, 32 },
				{ 192, 128, 64, 32 },
				{ 256, 128, 64, 32 } },
				0.1f
			};

			Animation bite{ "bite", {
				{ 0,   160, 64, 32 },
				{ 64,  160, 64, 32 },
				{ 128, 160, 64, 32 },
				{ 192, 160, 64, 32 },
				{ 256, 160, 64, 32 } },
				0.1f
			}; bite.Looping = false;

			Animator animator;
			animator.AddAnimation(idle);
			animator.AddAnimation(sleep);
			animator.AddAnimation(wake);
			animator.AddAnimation(alert);
			animator.AddAnimation(walk);
			animator.AddAnimation(run);
			animator.AddAnimation(bite);

			animator.SetCurrentAnimation("idle");
			AddComponent(m_Enemy, animator);
		}


		/*
		{
			m_Light2 = CreateEntity();

			AddComponent(m_Light2, DirectionalLight{ 0, 0, 0.6f, LightColor, true });
		}
		*/

		Physics::IgnoreCollisions("Player", "Enemies");
	}

private:
	Entity m_Ground = INVALID_ENTITY_ID;
	Entity m_JumpThroughPlatforms = INVALID_ENTITY_ID;
	
	Entity m_Ladders = INVALID_ENTITY_ID;
	Entity m_Crystals = INVALID_ENTITY_ID;
	
	Entity m_Camera = INVALID_ENTITY_ID;

	Entity m_Player = INVALID_ENTITY_ID;

	Entity m_Enemy = INVALID_ENTITY_ID;

	Entity m_Light = INVALID_ENTITY_ID;
	Entity m_Light2 = INVALID_ENTITY_ID;

};


class SandboxApp : public Application
{
public:
	SandboxApp()
		: Application("Sandbox", sf::Vector2i(1200, 675))
	{
		SetClearColor(sf::Color::Black);
		SetVSync(false);

		LoadScene<MainScene>();
	}
};

Application* SFMLEngine::CreateApplication()
{
	return new SandboxApp;
}
