#pragma once

#include <SFML/Graphics.hpp>

#include "ECS/Coordinator.h"

#include "Systems/RenderSystem.h"
#include "Systems/ScriptableEntitySystem.h"

#include "Systems/Lighting/PointLightSystem.h"
#include "Systems/Lighting/DirectionalLightSystem.h"

#include "Systems/GUISystem.h"
#include "Systems/CameraSystem.h"

#include "LayerStack.h"
#include "Scene.h"

namespace SFMLEngine {

	class Application
	{
	public:
		Application(const std::string& name, const sf::Vector2i& windowDimensions);
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

		std::shared_ptr<PointLightSystem> m_PointLightSystem;
		std::shared_ptr<DirectionalLightSystem> m_DirectionalLightSystem;

		std::shared_ptr<GUISystem> m_GUISystem;

		std::shared_ptr<CameraSystem> m_CameraSystem;

		sf::Vector2f m_DefaultWindowCentre;

	public:
		static Application* GetApplicationHandle() { return s_Instance; }

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
