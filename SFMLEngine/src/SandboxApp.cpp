#include "SFMLEngine.h"
#include "SFMLEngine/EntryPoint.h"

#include <SFML/Graphics.hpp>

// scripts
#include "GoToMouse.h"
#include "MoveByWASD.h"
#include "ClickToDestroyTile.h"
#include "ClickToPlace.h"
#include "PlayerController.h"


#include <cstdlib>
#include <ctime>


using namespace SFMLEngine;

class GameLayer : public Layer
{
public:
	GameLayer()
		: Layer()
	{
		srand(time(0));

		// create a new scene
		m_Scene = Application::GetApplicationHandle()->CreateScene();

		{
			m_Camera = m_Scene->CreateEntity();

			m_Scene->AddComponent(m_Camera, Transform{ sf::Vector2f(0, 0) });
			m_Scene->AddComponent(m_Camera, Camera{ });

			m_Scene->AddNativeScript<MoveByWASD>(m_Camera);
		}

		{

			m_Tilemap = m_Scene->CreateEntity();

			m_Scene->AddComponent(m_Tilemap, Transform{ sf::Vector2f(0, 300) });
			
			ResourceID tilePaletteID = TilePalette::Create(sf::Vector2u(64, 64));
			TilePalette* tilePalette = ResourceManager::GetResourceHandle<TilePalette>(tilePaletteID);

			TileID ground = tilePalette->CreateTile("ground", Texture::Create("assets/textures/ground.png"), Texture::Create("assets/textures/groundNormals.png"));
			TileID rocks = tilePalette->CreateTile("rocks", Texture::Create("assets/textures/rocks.png"), Texture::Create("assets/textures/rockNormals.png"));

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

			m_Scene->AddComponent(m_Tilemap, tilemapComponent);

			TilemapRenderer tilemapRendererComponent{ Material::Create("LitTilemap"), -1, 0 };
			m_Scene->AddComponent(m_Tilemap, tilemapRendererComponent);


			// this object should be solid
			// add a collider
			m_Scene->AddComponent(m_Tilemap, Collider{ ColliderType::Tilemap });
			m_Scene->AddComponent(m_Tilemap, TilemapCollider { });

			m_Scene->AddNativeScript<ClickToDestroyTile>(m_Tilemap);
		}

		{
			m_Background = m_Scene->CreateEntity();

			m_Scene->AddComponent(m_Background, Transform{ sf::Vector2f(0, -400) });

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

			m_Scene->AddComponent(m_Background, tilemapComponent);

			TilemapRenderer tilemapRendererComponent{ Material::Create("LitTilemap"), 0, 0 };
			m_Scene->AddComponent(m_Background, tilemapRendererComponent);
		}

		{
			// creating a second entity
			m_PhysicsEntity = m_Scene->CreateEntity();

			// give the entity a transform
			m_Scene->AddComponent(m_PhysicsEntity, Transform{ });
			// add a rigidbody so this entity is affected by physics
			m_Scene->AddComponent(m_PhysicsEntity, Rigidbody{ 0.1f });
			m_Scene->AddComponent(m_PhysicsEntity, Collider{ ColliderType::Box });
			m_Scene->AddComponent(m_PhysicsEntity, BoxCollider{ sf::Vector2f(34, 96), sf::Vector2f(16, 32) });

			// add the sprite renderer component
			m_Scene->AddComponent(m_PhysicsEntity, SpriteRenderer{
				Texture::Create("assets/textures/character.png"),
				Material::Create("Lit"),
				-2, 0,
				Texture::Create("assets/textures/characterNormals.png") });

			m_Scene->AddNativeScript<ClickToPlace>(m_PhysicsEntity);
			m_Scene->AddNativeScript<PlayerController>(m_PhysicsEntity);
		}

		{
			m_Light = m_Scene->CreateEntity();

			m_Scene->AddComponent(m_Light, Transform{ { 260, 300 } });
			m_Scene->AddComponent(m_Light, PointLight{ 3.0f, 0.007f, sf::Color{220, 130, 160, 255} });

			m_Scene->AddNativeScript<GoToMouse>(m_Light);
		}
		
		{
			m_Light2 = m_Scene->CreateEntity();

			m_Scene->AddComponent(m_Light2, DirectionalLight{ sf::Vector3f(1, 0, 0), 1.3f, sf::Color{94, 154, 220, 255}, true });
		}
	}

	~GameLayer()
	{
	}

	void GameLayer::OnAttach()
	{
		
	}

	void GameLayer::OnDetach()
	{
		m_Scene->DestroyEntity(m_Tilemap);
		m_Scene->DestroyEntity(m_Camera);

		m_Scene->DestroyEntity(m_Background);

		// m_Scene->DestroyEntity(m_PhysicsEntity);

		m_Scene->DestroyEntity(m_Light);
		m_Scene->DestroyEntity(m_Light2);
	}

	void GameLayer::OnEvent(sf::Event) {}

	void GameLayer::Update(Timestep ts)
	{

	}

private:
	std::shared_ptr<Scene> m_Scene;
	
	Entity m_Tilemap;
	Entity m_Background;

	Entity m_Camera;

	Entity m_PhysicsEntity;

	Entity m_Light;
	Entity m_Light2;
};



class SandboxApp : public Application
{
public:
	SandboxApp()
		: Application("Sandbox", sf::Vector2i(1200, 675))
	{
		PushLayer(new GameLayer());
	}
};

Application* SFMLEngine::CreateApplication()
{
	return new SandboxApp;
}
