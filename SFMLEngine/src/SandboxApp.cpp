#include "SFMLEngine.h"
#include "SFMLEngine/EntryPoint.h"

#include <SFML/Graphics.hpp>

// scripts
#include "Scripts/EntityScript.h"

#include <iostream>

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
				sf::Vector2f(100, 100),
				0,
				sf::Vector2f(1, 1)
			};
			// add the component
			m_Scene->AddComponent(m_Entity, entityTransform);

			// load a texture
			sf::Texture spriteTexture;
			if (!spriteTexture.loadFromFile("assets/textures/texture.png"))
				std::cout << "Failed to load texture!" << std::endl;
			std::cout << "texture created" << std::endl;
			sf::Sprite entitySprite;
			// add the sprite renderer component
			m_Scene->AddComponent(m_Entity, SpriteRenderer{ entitySprite, spriteTexture, 1 });

			// add the scripts onto the entity
			m_Scene->AddNativeScript<EntityScript>(m_Entity);
		}


		{
			// creating a second entity
			m_Entity2 = m_Scene->CreateEntity();


			// give the entity a transform
			Transform entityTransform{
				sf::Vector2f(132, 132),
				0,
				sf::Vector2f(1, 1)
			};
			// add the component
			m_Scene->AddComponent(m_Entity2, entityTransform);

			// load a texture
			sf::Texture spriteTexture;
			if (!spriteTexture.loadFromFile("assets/textures/texture2.png"))
				std::cout << "Failed to load texture!" << std::endl;
			sf::Sprite entitySprite;
			// add the sprite renderer component
			m_Scene->AddComponent(m_Entity2, SpriteRenderer{ entitySprite, spriteTexture, 0 });
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
