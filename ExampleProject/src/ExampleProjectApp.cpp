#include <SFMLEngine.h>
#include <SFMLEngine/EntryPoint.h> // mark this file as the entry point

#include <SFML/Graphics.hpp>

// scripts
#include "GoToEntity.h"
#include "GoToMouse.h"
#include "MoveByWASD.h"
#include "ClickToDestroyTile.h"
#include "ClickToPlace.h"
#include "PlayerController.h"
#include "SmoothFollowPlayer.h"
#include "ScrollToControlLight.h"


using namespace SFMLEngine;


const sf::Color LightColor{ 139, 145, 171, 255 };
const sf::Color BGColor{ 0, 15, 17, 255 };


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
			AddComponent(m_JumpThroughPlatforms, TilemapCollider{ TilemapCollider::OptimizationLevel::High });
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

			Animator animator;
			animator.AddAnimation(idle);
			animator.AddAnimation(run);
			animator.AddAnimation(jump);
			animator.AddAnimation(punch);
			animator.AddAnimation(climb);

			animator.SetCurrentAnimation("idle");
			AddComponent(m_Player, animator);
			

			AddNativeScript<PlayerController>(m_Player);
		}

		{
			m_Light = CreateEntity();

			// light is a child transform of physics entity
			AddComponent(m_Light, Transform{ sf::Vector2f(16, 32), & GetComponent<Transform>(m_Player) });
			AddComponent(m_Light, PointLight{ 1.3f, 100.0f, sf::Color(219, 113, 114, 255) });

			AddNativeScript<ScrollToControlLight>(m_Light);
		}

		{
			m_Camera = CreateEntity();

			AddComponent(m_Camera, Transform{ });

			Camera cam{ };
			cam.Zoom = 0.5f;
			AddComponent(m_Camera, cam);

			AddNativeScript<SmoothFollowPlayer>(m_Camera);
		}
		
		{
			m_Light2 = CreateEntity();

			AddComponent(m_Light2, DirectionalLight{ 0, 0, 0.6f, LightColor, true });
		}
	}

private:
	Entity m_Ground = INVALID_ENTITY_ID;
	Entity m_JumpThroughPlatforms = INVALID_ENTITY_ID;
	
	Entity m_Ladders = INVALID_ENTITY_ID;
	
	Entity m_Camera = INVALID_ENTITY_ID;

	Entity m_Player = INVALID_ENTITY_ID;

	Entity m_Light = INVALID_ENTITY_ID;
	Entity m_Light2 = INVALID_ENTITY_ID;
};


class SandboxApp : public Application
{
public:
	SandboxApp()
		: Application("Sandbox", sf::Vector2i(1200, 675))
	{
		SetClearColor(BGColor * LightColor);

		LoadScene<MainScene>();
	}
};

Application* SFMLEngine::CreateApplication()
{
	return new SandboxApp;
}
