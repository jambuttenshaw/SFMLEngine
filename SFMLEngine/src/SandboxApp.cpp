#include "SFMLEngine.h"
#include "SFMLEngine/EntryPoint.h"

#include <SFML/Graphics.hpp>

// scripts
#include "Scripts/GoToMouse.h"
#include "Scripts/MoveByWASD.h"
#include "Scripts/ClickToDestroyTile.h"
#include "Scripts/ClickToPlace.h"


using namespace SFMLEngine;

class GameLayer : public Layer
{
public:
	GameLayer()
		: Layer()
	{
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

			TileID cobblestone = tilePalette->CreateTile("cobblestone", Texture::Create("assets/textures/cobblestoneTexture.png"), Texture::Create("assets/textures/cobblestoneNormal.png"));
			TileID mossyCobblestone = tilePalette->CreateTile("mossyCobblestone", Texture::Create("assets/textures/mossyCobblestoneTexture.png"), Texture::Create("assets/textures/cobblestoneNormal.png"));

			Tilemap tilemapComponent{ tilePaletteID };

			// place tiles into the tilemap
			int numTiles = 16;
			for (int x = 0; x < numTiles; x++)
			{
				for (int y = 0; y < 2; y++)
				{
					tilemapComponent.PlaceTile(sf::Vector2i(x - numTiles / 2, y), cobblestone);
				}
			}
			tilemapComponent.PlaceTile(sf::Vector2i(5, -1), mossyCobblestone);
			tilemapComponent.PlaceTile(sf::Vector2i(4, -1), mossyCobblestone);
			tilemapComponent.PlaceTile(sf::Vector2i(4, -2), mossyCobblestone);

			m_Scene->AddComponent(m_Tilemap, tilemapComponent);

			TilemapRenderer tilemapRendererComponent{ Material::Create("LitTilemap"), 1, 0 };
			m_Scene->AddComponent(m_Tilemap, tilemapRendererComponent);


			// this object should be solid
			// add a collider
			m_Scene->AddComponent(m_Tilemap, Collider{ ColliderType::Tilemap });
			//m_Scene->AddComponent(m_Tilemap, BoxCollider{ sf::Vector2f(1024, 128), sf::Vector2f(-544, -32) });
			m_Scene->AddComponent(m_Tilemap, TilemapCollider { &tilemapComponent });

			m_Scene->AddNativeScript<ClickToDestroyTile>(m_Tilemap);
		}

		{
			// creating a second entity
			m_PhysicsEntity = m_Scene->CreateEntity();

			// give the entity a transform
			m_Scene->AddComponent(m_PhysicsEntity, Transform{ sf::Vector2f(0, -300) });
			// add a rigidbody so this entity is affected by physics
			m_Scene->AddComponent(m_PhysicsEntity, Rigidbody{ });
			m_Scene->AddComponent(m_PhysicsEntity, Collider{ ColliderType::Box });
			m_Scene->AddComponent(m_PhysicsEntity, BoxCollider{ sf::Vector2f(64, 64), sf::Vector2f(-32, -32) });

			// add the sprite renderer component
			m_Scene->AddComponent(m_PhysicsEntity, SpriteRenderer{
				Texture::Create("assets/textures/cobblestoneTexture.png"),
				Material::Create("Lit"),
				-1, 0,
				Texture::Create("assets/textures/cobblestoneNormal.png") });

			m_Scene->AddNativeScript<ClickToPlace>(m_PhysicsEntity);
		}

		{
			m_Light = m_Scene->CreateEntity();

			m_Scene->AddComponent(m_Light, Transform{ { 260, 300 } });
			m_Scene->AddComponent(m_Light, PointLight{ 3.0f, 0.007f, sf::Color{220, 130, 160, 255} });
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

		m_Scene->DestroyEntity(m_PhysicsEntity);

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
