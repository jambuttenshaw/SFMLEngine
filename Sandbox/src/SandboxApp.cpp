#include <SFMLEngine.h>
#include <SFMLEngine/EntryPoint.h>

#include <SFML/Graphics.hpp>

#include <iostream>

class GameLayer : public SFMLEngine::Layer
{
public:
	GameLayer()
		: Layer()
	{
		// create a new scene
		m_Scene = SFMLEngine::Application::GetApplicationHandle()->CreateScene();
		
		// create a new entity
		m_Entity = m_Scene->CreateEntity();		
	}

	~GameLayer() 
	{
		m_Scene->DestroyEntity(m_Entity);
	}

	void GameLayer::OnAttach() {}
	void GameLayer::OnDetach() {}

	void GameLayer::OnEvent(sf::Event) {}

	void GameLayer::Update(float timestep)
	{

	}

private:
	std::shared_ptr<SFMLEngine::Scene> m_Scene;

	SFMLEngine::Entity m_Entity;
};



class SandboxApp : public SFMLEngine::Application
{
public:
	SandboxApp()
		: Application()
	{
		PushLayer(new GameLayer());
	}
};

SFMLEngine::Application* SFMLEngine::CreateApplication()
{
	return new SandboxApp;
}
