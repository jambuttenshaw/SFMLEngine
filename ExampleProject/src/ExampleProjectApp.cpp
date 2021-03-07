#include "SFMLEngine.h"

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

class MainScene : public Scene
{
public:
	void Create() override
	{
		{

			m_Tilemap = CreateEntity();

			AddComponent(m_Tilemap, Transform{ sf::Vector2f(0, 0) });
			
			ResourceID tilePaletteID = TilePalette::LoadFromFile("assets/palettes/terrainPalette.json");
			TilePalette* tilePalette = ResourceManager::GetResourceHandle<TilePalette>(tilePaletteID);

			// load a tilemap from a file
			Tilemap tilemapComponent{ tilePaletteID, "assets/tilemaps/terrain.json" };

			AddComponent(m_Tilemap, tilemapComponent);

			TilemapRenderer tilemapRendererComponent{ Material::Create("LitTilemap"), 1 };
			AddComponent(m_Tilemap, tilemapRendererComponent);


			// this object should be solid
			// add a collider
			AddComponent(m_Tilemap, TilemapCollider { TilemapCollider::OptimizationLevel::Standard });
			AddComponent(m_Tilemap, ColliderInfo{ ColliderType::Tilemap });

			AddNativeScript<ClickToDestroyTile>(m_Tilemap);
		}


		{
			m_Background = CreateEntity();

			AddComponent(m_Background, Transform{ sf::Vector2f(0, 0) });

			ResourceID tilePaletteID = TilePalette::Create(sf::Vector2u(256, 256));
			TilePalette* tilePalette = ResourceManager::GetResourceHandle<TilePalette>(tilePaletteID);

			TileID background = tilePalette->CreateTile("background", Texture::Create("assets/textures/background.png"), Texture::Create("assets/textures/backgroundNormals.png"));


			Tilemap tilemapComponent{ tilePaletteID };

			// place tiles into the tilemap
			int numTiles = 32;
			for (int x = 0; x < numTiles; x++)
			{
				for (int y = 0; y < 5; y++)
				{
					tilemapComponent.PlaceTile(sf::Vector2i(x - numTiles / 2, 1 - y), background);
				}
			}

			AddComponent(m_Background, tilemapComponent);

			TilemapRenderer tilemapRendererComponent{ Material::Create("LitTilemap"), 0 };
			AddComponent(m_Background, tilemapRendererComponent);
		}
		

		{
			// creating a second entity
			m_PhysicsEntity = CreateEntity("Player", "Player", "Player");

			// give the entity a transform
			AddComponent(m_PhysicsEntity, Transform{ sf::Vector2f(0, -100) });
			// add a rigidbody so this entity is affected by physics
			AddComponent(m_PhysicsEntity, Rigidbody{ });
			AddComponent(m_PhysicsEntity, BoxCollider{ sf::Vector2f(17, 48), sf::Vector2f(8, 16) });
			AddComponent(m_PhysicsEntity, ColliderInfo{ ColliderType::Box });

			// add the sprite renderer component
			AddComponent(m_PhysicsEntity, SpriteRenderer{
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

			Animator animator;
			animator.AddAnimation(idle);
			animator.AddAnimation(run);
			animator.AddAnimation(jump);

			animator.SetCurrentAnimation("idle");
			AddComponent(m_PhysicsEntity, animator);
			

			AddNativeScript<PlayerController>(m_PhysicsEntity);
		}

		{
			m_Light = CreateEntity();

			// light is a child transform of physics entity
			AddComponent(m_Light, Transform{ sf::Vector2f(16, 32), & GetComponent<Transform>(m_PhysicsEntity) });
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

			AddComponent(m_Light2, DirectionalLight{ 0, 0, 0.6f, sf::Color{94, 62, 180, 255}, true });
		}
	}

private:
	Entity m_Tilemap = INVALID_ENTITY_ID;
	Entity m_Background = INVALID_ENTITY_ID;

	Entity m_Camera = INVALID_ENTITY_ID;

	Entity m_PhysicsEntity = INVALID_ENTITY_ID;

	Entity m_Light = INVALID_ENTITY_ID;
	Entity m_Light2 = INVALID_ENTITY_ID;
};



class SandboxApp : public Application
{
public:
	SandboxApp()
		: Application("Sandbox", sf::Vector2i(1200, 675))
	{
		SetClearColor(sf::Color::Magenta);

		LoadScene<MainScene>();
	}
};

Application* SFMLEngine::CreateApplication()
{
	return new SandboxApp;
}
