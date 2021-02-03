#include "SFMLEngine.h"
#include "SFMLEngine/EntryPoint.h"

#include <SFML/Graphics.hpp>

// scripts
#include "Scripts/EntityScript.h"
#include "Scripts/LightMovement.h"


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
			// this time a default transform
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

			mat->SetProperty("u_Lights[0].Position", sf::Vector3f(128, 128, 5));
			mat->SetProperty("u_Lights[0].Intensity", 3.0f);
			mat->SetProperty("u_Lights[0].Range", 0.007f);
			mat->SetProperty("u_Lights[0].Color", sf::Color(204, 51, 51));


			// add the sprite renderer component
			m_Scene->AddComponent(m_Entity, SpriteRenderer{ spriteTexture, materialID, 2, 0 });

			// add a script to move the light
			auto& script = m_Scene->AddNativeScript<LightMovement>(m_Entity);
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
