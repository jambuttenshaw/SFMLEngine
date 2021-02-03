#include "SFMLEngine.h"
#include "SFMLEngine/EntryPoint.h"

#include <SFML/Graphics.hpp>

// scripts
#include "Scripts/LightMovement.h"
#include "Scripts/GoToMouse.h"


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
			// creating a second entity
			m_Entity = m_Scene->CreateEntity();

			// give the entity a transform
			m_Scene->AddComponent(m_Entity, Transform{ sf::Vector2f(272, 172), 0, sf::Vector2f(4, 4) });
			// add the sprite renderer component
			m_Scene->AddComponent(m_Entity, SpriteRenderer{
				Texture::Create("assets/textures/texture2.png"),
				Material::Create("Lit"),
				2, 0,
				Texture::Create("assets/textures/textureNormalMap.png") });
		}


		{
			// creating a second entity
			m_Entity2 = m_Scene->CreateEntity();

			// give the entity a transform
			m_Scene->AddComponent(m_Entity2, Transform{ sf::Vector2f(100, 300), 0, sf::Vector2f(2, 2) });
			// add the sprite renderer component
			m_Scene->AddComponent(m_Entity2, SpriteRenderer{
				Texture::Create("assets/textures/texture.png"),
				Material::Create("Lit"),
				0, 0,
				Texture::Create("assets/textures/textureNormalMap.png") });
		}


		{
			m_Light = m_Scene->CreateEntity();

			m_Scene->AddComponent(m_Light, Transform{});
			m_Scene->AddComponent(m_Light, PointLight{ 2.0f, 0.007f, sf::Color{200, 50, 50, 255} });

			m_Scene->AddNativeScript<GoToMouse>(m_Light);
		}
		
		{
			m_Light2 = m_Scene->CreateEntity();

			m_Scene->AddComponent(m_Light2, Transform{sf::Vector2f(530, 420)});
			m_Scene->AddComponent(m_Light2, PointLight{ 4.0f, 0.007f, sf::Color{50, 50, 200, 255} });
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
		m_Scene->DestroyEntity(m_Entity2);

		m_Scene->DestroyEntity(m_Light);
		m_Scene->DestroyEntity(m_Light2);
	}

	void GameLayer::OnEvent(sf::Event) {}

	void GameLayer::Update(Timestep ts)
	{

	}

private:
	std::shared_ptr<Scene> m_Scene;
	
	Entity m_Entity;
	Entity m_Entity2;

	Entity m_Light;
	Entity m_Light2;
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
