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
			Transform entityTransform{sf::Vector2f(272, 172), 0, sf::Vector2f(4, 4)};
			// add the component
			m_Scene->AddComponent(m_Entity, entityTransform);


			// load a texture
			ResourceID spriteTexture = Texture::Create("assets/textures/texture.png");


			// create a material from a shader
			ResourceID materialID = Material::CreateInstance("Lit");
			Material* mat = ResourceManager::GetResourceHandle<Material>(materialID);

			// set material properties
			ResourceID normalMap = Texture::Create("assets/textures/textureNormalMap.png");
			mat->SetProperty("u_NormalMap", normalMap);


			// add the sprite renderer component
			m_Scene->AddComponent(m_Entity, SpriteRenderer{ spriteTexture, materialID, 2, 0 });
		}


		{
			m_Light = m_Scene->CreateEntity();

			m_Scene->AddComponent(m_Light, Transform{});
			m_Scene->AddComponent(m_Light, PointLight{ 2.0f, 0.007f, sf::Color::Red });

			m_Scene->AddNativeScript<GoToMouse>(m_Light);
		}
		
		{
			m_Light2 = m_Scene->CreateEntity();

			m_Scene->AddComponent(m_Light2, Transform{sf::Vector2f(400, 300)});
			m_Scene->AddComponent(m_Light2, PointLight{ 4.0f, 0.007f, sf::Color::Blue });
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
