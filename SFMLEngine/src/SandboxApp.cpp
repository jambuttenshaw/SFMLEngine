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

		{
			m_Camera = m_Scene->CreateEntity();

			m_Scene->AddComponent(m_Camera, Transform{ sf::Vector2f(0, 0) });
			m_Scene->AddComponent(m_Camera, Camera{ });
		}

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

				m_Tiles.push_back(entity);
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

			m_Scene->AddComponent(m_Light2, DirectionalLight{ sf::Vector3f(1, 0, 0), 0.4f, sf::Color{32, 220, 70, 255}, true });
		}

		{
			m_Text = m_Scene->CreateEntity();

			m_Scene->AddComponent(m_Text, Transform{ sf::Vector2f(200.0f, 20.0f) });
			m_Scene->AddComponent(m_Text, Text{ FontLibrary::GetFont("arial"), "This is some text", 30, sf::Color::Red });
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
		m_Scene->DestroyEntity(m_Camera);

		for (auto const& e : m_Tiles)
			m_Scene->DestroyEntity(e);

		m_Scene->DestroyEntity(m_Light);
		m_Scene->DestroyEntity(m_Light2);

		m_Scene->DestroyEntity(m_Text);
	}

	void GameLayer::OnEvent(sf::Event) {}

	void GameLayer::Update(Timestep ts)
	{

	}

private:
	std::shared_ptr<Scene> m_Scene;
	
	Entity m_Camera;

	std::vector<Entity> m_Tiles;

	Entity m_Light;
	Entity m_Light2;

	Entity m_Text;
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
