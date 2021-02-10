#pragma once

#include <SFML/Graphics.hpp>

#include "ECS/Coordinator.h"


// systems

#include "Systems/SpriteRenderSystem.h"
#include "Systems/TilemapRenderSystem.h"

#include "Systems/ScriptableEntitySystem.h"

#include "Systems/Lighting/PointLightSystem.h"
#include "Systems/Lighting/DirectionalLightSystem.h"

#include "Systems/GUISystem.h"

#include "Systems/CameraSystem.h"

#include "Systems/CollisionSystem.h"
#include "Systems/PhysicsSystem.h"


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
		sf::Vector2u GetWindowDimensions() { return m_Window->getSize(); }

	private:
		sf::RenderWindow* m_Window;

		sf::Clock m_Clock;

		LayerStack m_LayerStack;

		// ECS
		Coordinator* m_Coordinator;

		// ECS systems
		std::shared_ptr<SpriteRenderSystem> m_SpriteRenderSystem;
		std::shared_ptr<TilemapRenderSystem> m_TilemapRenderSystem;

		std::shared_ptr<ScriptableEntitySystem> m_ScriptableEntitySystem;

		std::shared_ptr<PointLightSystem> m_PointLightSystem;
		std::shared_ptr<DirectionalLightSystem> m_DirectionalLightSystem;

		std::shared_ptr<GUISystem> m_GUISystem;

		std::shared_ptr<CameraSystem> m_CameraSystem;

		std::shared_ptr<CollisionSystem> m_CollisionSystem;
		std::shared_ptr<PhysicsSystem> m_PhysicsSystem;


		// Debug mode stuff
		bool m_DisplayDebug = false;
		sf::Text m_DebugText;
		std::vector<std::string> m_DebugInfo;

	public:
		static Application* GetApplicationHandle() { return s_Instance; }
		static sf::Vector2u GetWindowSize() { return GetApplicationHandle()->GetWindowDimensions(); }

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
