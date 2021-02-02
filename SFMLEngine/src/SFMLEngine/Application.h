#pragma once

#include <SFML/Graphics.hpp>

#include "ECS/Coordinator.h"

#include "Systems/RenderSystem.h"
#include "Systems/ScriptableEntitySystem.h"
#include "Systems/LightingSystem.h"

#include "LayerStack.h"
#include "Scene.h"

namespace SFMLEngine {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void Shutdown();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		sf::RenderWindow* GetWindowHandle() { return m_Window; }

		std::shared_ptr<Scene> CreateScene() { return std::make_shared<Scene>(m_Coordinator, m_ScriptableEntitySystem); }

	private:
		sf::RenderWindow* m_Window;

		sf::Clock m_Clock;

		LayerStack m_LayerStack;

		// ECS
		Coordinator* m_Coordinator;

		// ECS systems
		std::shared_ptr<RenderSystem> m_RenderSystem;
		std::shared_ptr<ScriptableEntitySystem> m_ScriptableEntitySystem;
		std::shared_ptr<LightingSystem> m_LightingSystem;

	public:
		static Application* GetApplicationHandle() { return s_Instance; }

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
