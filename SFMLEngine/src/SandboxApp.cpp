#include "SFMLEngine.h"
#include "SFMLEngine/EntryPoint.h"

#include <SFML/Graphics.hpp>

// scripts
#include "Scripts/EntityScript.h"


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
			// create an entity
			m_Entity = m_Scene->CreateEntity();


			// give the entity a transform
			Transform entityTransform{
				sf::Vector2f(50, 50),
				0,
				sf::Vector2f(1, 1)
			};
			// add the component
			m_Scene->AddComponent(m_Entity, entityTransform);

			// load a texture
			ResourceID spriteTexture = ResourceManager::LoadFromFile<sf::Texture>("assets/textures/texture.png");
			ResourceID materialID = Material::Create("Basic");
			Material* mat = ResourceManager::GetResourceHandle<Material>(materialID);

			mat->SetUniform("u_Color", sf::Glsl::Vec4(sf::Color::Red));

			// add the sprite renderer component
			m_Scene->AddComponent(m_Entity, SpriteRenderer{ spriteTexture, Material::Create("Basic"), -1 });

			// add the scripts onto the entity
			m_Scene->AddNativeScript<EntityScript>(m_Entity);
		}


		{
			// creating a second entity
			m_Entity2 = m_Scene->CreateEntity();


			// give the entity a transform
			// this time a default transform
			Transform entityTransform{};
			// add the component
			m_Scene->AddComponent(m_Entity2, entityTransform);

			// load a texture
			ResourceID spriteTexture = ResourceManager::LoadFromFile<sf::Texture>("assets/textures/texture2.png");

			// add the sprite renderer component
			m_Scene->AddComponent(m_Entity2, SpriteRenderer{ spriteTexture, Material::Create("Basic"), 0 });
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
		m_Scene->DestroyEntity(m_Entity);
	}

	void GameLayer::OnEvent(sf::Event) {}

	void GameLayer::Update(Timestep ts)
	{

	}

private:
	std::shared_ptr<Scene> m_Scene;
	
	Entity m_Entity;
	Entity m_Entity2;
};



class SandboxApp : public Application
{
public:
	SandboxApp()
		: Application()
	{
		PushLayer(new GameLayer());
	}
};

Application* SFMLEngine::CreateApplication()
{
	return new SandboxApp;
}
