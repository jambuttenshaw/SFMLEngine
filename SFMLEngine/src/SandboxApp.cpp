#include "SFMLEngine.h"
#include "SFMLEngine/EntryPoint.h"

#include <SFML/Graphics.hpp>

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

		m_Entity = m_Scene->CreateEntity();

		Transform entityTransform{
			sf::Vector2f(100, 100),
			0,
			sf::Vector2f(1, 1)
		};
		m_Scene->AddComponent(m_Entity, entityTransform);

		sf::Texture spriteTexture;
		if (!spriteTexture.loadFromFile("assets/texture.png"))
			std::cout << "Failed to load texture!" << std::endl;
		sf::Sprite entitySprite(spriteTexture);
		m_Scene->AddComponent(m_Entity, SpriteRenderer{ entitySprite });
			
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

	void GameLayer::Update(float timestep)
	{

	}

private:
	std::shared_ptr<Scene> m_Scene;
	Entity m_Entity;
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
