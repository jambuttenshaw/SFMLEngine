#include "SFMLEngine.h"
#include "SFMLEngine/EntryPoint.h"

#include <SFML/Graphics.hpp>

#include <iostream>

namespace SFMLEngine {

	class GameLayer : public Layer
	{
	public:
		GameLayer()
			: Layer()
		{
			// create a new scene
			m_Scene = Application::GetApplicationHandle()->CreateScene();

			m_Entity = m_Scene->CreateEntity();
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

	Application* CreateApplication()
	{
		return new SandboxApp;
	}

}
