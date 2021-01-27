#pragma once

#include <SFML/Graphics.hpp>

#include "ECS/Coordinator.h"
#include "Systems/RenderSystem.h"

#include "LayerStack.h"
#include "Scene.h"

namespace SFMLEngine {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		sf::RenderWindow* GetWindowHandle() { return m_Window; }

		std::shared_ptr<Scene> CreateScene() { return std::make_shared<Scene>(m_Coordinator); }

	private:
		sf::RenderWindow* m_Window;

		LayerStack m_LayerStack;

		// ECS
		Coordinator* m_Coordinator;

		// ECS systems
		std::shared_ptr<RenderSystem> m_RenderSystem;

	public:
		static Application* GetApplicationHandle() { return s_Instance; }

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
