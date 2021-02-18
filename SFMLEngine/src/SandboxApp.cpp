#include "SFMLEngine.h"
#include "SFMLEngine/EntryPoint.h"

#include <SFML/Graphics.hpp>

// scripts
#include "GoToEntity.h"
#include "MoveByWASD.h"
#include "ClickToDestroyTile.h"
#include "ClickToPlace.h"
#include "PlayerController.h"
#include "SmoothFollowPlayer.h"


using namespace SFMLEngine;

class MainScene : public Scene
{
public:
	void Create() override
	{
		{

			m_Tilemap = CreateEntity();

			AddComponent(m_Tilemap, Transform{ sf::Vector2f(0, 300) });
			
			ResourceID tilePaletteID = TilePalette::LoadFromFile("assets/textures/terrain.png", "assets/textures/terrainNormals.png", sf::Vector2u(32, 32));
			TilePalette* tilePalette = ResourceManager::GetResourceHandle<TilePalette>(tilePaletteID);


			TileID rocks = tilePalette->GetTileByName("Tile0");
			TileID ground = tilePalette->GetTileByName("Tile1");


			Tilemap tilemapComponent{ tilePaletteID };

			// place tiles into the tilemap
			int numTiles = 64;
			for (int x = 0; x < numTiles; x++)
			{
				int yOff = rand() % 3;
				tilemapComponent.PlaceTile(sf::Vector2i(x - numTiles / 2, yOff), ground);
				
				for (int y = 1; y < 10 - yOff; y++)
				{
					tilemapComponent.PlaceTile(sf::Vector2i(x - numTiles / 2, yOff + y), rocks);
				}
				
			}

			AddComponent(m_Tilemap, tilemapComponent);

			TilemapRenderer tilemapRendererComponent{ Material::Create("LitTilemap"), -1, 0 };
			AddComponent(m_Tilemap, tilemapRendererComponent);


			// this object should be solid
			// add a collider
			AddComponent(m_Tilemap, Collider{ ColliderType::Tilemap });
			AddComponent(m_Tilemap, TilemapCollider { });

			AddNativeScript<ClickToDestroyTile>(m_Tilemap);
		}


		{
			m_Background = CreateEntity();

			AddComponent(m_Background, Transform{ sf::Vector2f(0, -400) });

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
					tilemapComponent.PlaceTile(sf::Vector2i(x - numTiles / 2, y), background);
				}
			}

			AddComponent(m_Background, tilemapComponent);

			TilemapRenderer tilemapRendererComponent{ Material::Create("LitTilemap"), 0, 0 };
			AddComponent(m_Background, tilemapRendererComponent);
		}


		{
			// creating a second entity
			m_PhysicsEntity = CreateEntity("Player", "Player", "Player");

			// give the entity a transform
			AddComponent(m_PhysicsEntity, Transform{ sf::Vector2f(0, -500) });
			// add a rigidbody so this entity is affected by physics
			AddComponent(m_PhysicsEntity, Rigidbody{ 0.1f });
			AddComponent(m_PhysicsEntity, Collider{ ColliderType::Box });
			AddComponent(m_PhysicsEntity, BoxCollider{ sf::Vector2f(17, 48), sf::Vector2f(8, 16) });

			// add the sprite renderer component
			AddComponent(m_PhysicsEntity, SpriteRenderer{
				Texture::Create("assets/textures/character.png"),
				Material::Create("Lit"),
				-2, 0,
				Texture::Create("assets/textures/characterNormals.png") });

			AddNativeScript<ClickToPlace>(m_PhysicsEntity);
			AddNativeScript<PlayerController>(m_PhysicsEntity);
		}

		{
			m_Light = CreateEntity();

			// light is a child transform of physics entity
			AddComponent(m_Light, Transform{ sf::Vector2f(16, 32), &GetComponent<Transform>(m_PhysicsEntity) });
			AddComponent(m_Light, PointLight{ 1.3f, 0.007f, sf::Color(219, 113, 114, 255) });

		}

		{
			m_Camera = CreateEntity();

			AddComponent(m_Camera, Transform{ });

			Camera cam{ };
			cam.Zoom = 0.5f;
			AddComponent(m_Camera, cam);

			auto& script = AddNativeScript<SmoothFollowPlayer>(m_Camera);
			script.SetPlayerTransform(&GetComponent<Transform>(m_PhysicsEntity));
		}
		
		{
			m_Light2 = CreateEntity();

			AddComponent(m_Light2, DirectionalLight{ sf::Vector3f(1, 0, 0), 0.6f, sf::Color{94, 62, 180, 255}, true });
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
		LoadScene<MainScene>();
	}
};

Application* SFMLEngine::CreateApplication()
{
	return new SandboxApp;
}
