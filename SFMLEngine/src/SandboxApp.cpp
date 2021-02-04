#include "SFMLEngine.h"
#include "SFMLEngine/EntryPoint.h"

#include <SFML/Graphics.hpp>

// scripts
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

		for (int x = 0; x < 10; x++)
		{
			for (int y = 0; y < 10; y++)
			{
				// creating a second entity
				Entity entity = m_Scene->CreateEntity();

				// give the entity a transform
				m_Scene->AddComponent(entity, Transform{ sf::Vector2f((float)(x * 64), (float)(536 - y * 64)) });
				// add the sprite renderer component
				m_Scene->AddComponent(entity, SpriteRenderer{
					Texture::Create("assets/textures/cobblestoneTexture.png"),
					Material::Create("Lit"),
					0, 0,
					Texture::Create("assets/textures/cobblestoneNormal.png") });
			}
		}

		{
			m_Light = m_Scene->CreateEntity();

			m_Scene->AddComponent(m_Light, Transform{});
			m_Scene->AddComponent(m_Light, PointLight{ 2.0f, 0.007f, sf::Color{180, 120, 120, 255} });

			m_Scene->AddNativeScript<GoToMouse>(m_Light);
		}
		
		{
			m_Light2 = m_Scene->CreateEntity();

			m_Scene->AddComponent(m_Light2, Transform{sf::Vector2f(360, 350)});
			m_Scene->AddComponent(m_Light2, PointLight{ 4.0f, 0.007f, sf::Color{100, 100, 200, 255} });
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
		for (auto const& e : m_Tiles)
			m_Scene->DestroyEntity(e);
		

		m_Scene->DestroyEntity(m_Light);
		m_Scene->DestroyEntity(m_Light2);
	}

	void GameLayer::OnEvent(sf::Event) {}

	void GameLayer::Update(Timestep ts)
	{

	}

private:
	std::shared_ptr<Scene> m_Scene;
	
	std::vector<Entity> m_Tiles;

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
