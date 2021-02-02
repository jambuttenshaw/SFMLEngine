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

		/*{
			// create an entity
			m_Entity = m_Scene->CreateEntity();


			// give the entity a transform
			Transform entityTransform{
				sf::Vector2f(50, 50),
				0,
				sf::Vector2f(0.25f, 0.25f)
			};
			// add the component
			m_Scene->AddComponent(m_Entity, entityTransform);


			// load a texture
			ResourceID spriteTexture = Texture::Create("assets/textures/texture.png");


			// create a material from a shader
			ResourceID materialID = Material::CreateInstance("Basic");
			Material* mat = ResourceManager::GetResourceHandle<Material>(materialID);
			// set material properties
			mat->SetUniform("u_Color", sf::Glsl::Vec4(sf::Color::Red));


			// add the sprite renderer component
			m_Scene->AddComponent(m_Entity, SpriteRenderer{ spriteTexture, materialID, 1, 0 });

			// add the scripts onto the entity
			m_Scene->AddNativeScript<EntityScript>(m_Entity);
		}*/




		{
			// creating a second entity
			m_Entity2 = m_Scene->CreateEntity();


			// give the entity a transform
			// this time a default transform
			Transform entityTransform{sf::Vector2f(272, 172), 0, sf::Vector2f(4, 4)};
			// add the component
			m_Scene->AddComponent(m_Entity2, entityTransform);


			// load a texture
			ResourceID spriteTexture = Texture::Create("assets/textures/texture2.png");


			// create a material from a shader
			ResourceID materialID = Material::CreateInstance("Lit");
			Material* mat = ResourceManager::GetResourceHandle<Material>(materialID);
			// set material properties
			ResourceID normalMap = Texture::Create("assets/textures/textureNormalMap.png");
			mat->SetUniform("u_NormalMap", normalMap);

			mat->SetUniform("u_LightPos", sf::Glsl::Vec3(128, 128, -5));


			// add the sprite renderer component
			m_Scene->AddComponent(m_Entity2, SpriteRenderer{ spriteTexture, materialID, 2, 0 });

			// add a script to move the light
			auto& script = m_Scene->AddNativeScript<LightMovement>(m_Entity2);
			//script.SetEntityScript(&m_Scene->GetNativeScript<EntityScript>(m_Entity));
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
		//m_Scene->DestroyEntity(m_Entity);
		m_Scene->DestroyEntity(m_Entity2);
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
