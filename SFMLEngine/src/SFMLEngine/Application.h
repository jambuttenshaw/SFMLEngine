#pragma once

#include <SFML/Graphics.hpp>

#include <type_traits>


#include "ECS/Coordinator.h"

// systems

#include "Systems/IdentitySystem.h"

#include "Systems/Render/SpriteRenderSystem.h"
#include "Systems/Render/TilemapRenderSystem.h"

#include "Systems/ScriptableEntitySystem.h"

#include "Systems/Lighting/PointLightSystem.h"
#include "Systems/Lighting/DirectionalLightSystem.h"

#include "Systems/GUISystem.h"

#include "Systems/CameraSystem.h"

#include "Systems/Physics/CollisionSystem.h"
#include "Systems/Physics/PhysicsSystem.h"

#include "Systems/Physics/TilemapSystem.h"
#include "Systems/Physics/BoxColliderDebugSystem.h"

#include "Systems/AnimationSystem.h"


#include "Scene.h"


/*
An SFMLEngine Application is the main game run by the engine

The Application class should be extended by the user to add their content to the game
Then the user should define CreateApplication that instantiates their application

Interaction with the SFML window and Scene manager is conducted through the application class
Only 1 instance of Application will ever exist; it is a singleton. This is asserted upon instantiation.
*/


namespace SFMLEngine {

	class Application
	{
	public:
		Application(const std::string& name, const sf::Vector2i& windowDimensions);
		virtual ~Application();

		void Run();

		void Shutdown();

		
		sf::RenderWindow* GetWindowHandle() { return m_Window; }


		void SetVSync(bool state);
		inline bool IsVSync() { return m_VSync; }

		void SetFPSLimit(unsigned int limit);
		unsigned int GetFPSLimit() { return m_FPSLimit; }

		void SetClearColor(const sf::Color& color) { m_ClearColor = color; }
		const sf::Color& GetClearColor() { return m_ClearColor; }


		template <typename T>
		void LoadScene()
		{
			// T MUST inherit from Scene
			static_assert(std::is_base_of<Scene, T>::value, "T should inherit from Scene.");

			// check if theres a scene already loaded
			if (m_CurrentScene)
			{
				// clear out all of the existing entities
				m_CurrentScene->Destroy();
				delete m_CurrentScene;
			}

			T* newScene = new T;
			// implicit conversion to Scene* since T inherits from Scene
			m_CurrentScene = newScene;

			newScene->Init(m_Coordinator, m_ScriptableEntitySystem, m_IdentitySystem);

			// pure virtual function that will create a scene T
			newScene->Create();
		}

	private:
		sf::Vector2u GetWindowDimensions() { return m_Window->getSize(); }

	private:
		sf::RenderWindow* m_Window;
		// a view with topleft at 0,0 and size = window size
		sf::View m_GUIView;

		sf::Clock m_Clock;

		Scene* m_CurrentScene = nullptr;

		// VSync enabled by default
		bool m_VSync = true;
		unsigned int m_FPSLimit = 0;
		const float m_DeltaTimeLimit = 1.0f / 20.0f;

		sf::Color m_ClearColor{ sf::Color::Black };

		// ECS
		Coordinator* m_Coordinator;

		// ECS systems
		std::shared_ptr<IdentitySystem> m_IdentitySystem;

		std::shared_ptr<SpriteRenderSystem> m_SpriteRenderSystem;
		std::shared_ptr<TilemapRenderSystem> m_TilemapRenderSystem;

		std::shared_ptr<ScriptableEntitySystem> m_ScriptableEntitySystem;

		std::shared_ptr<PointLightSystem> m_PointLightSystem;
		std::shared_ptr<DirectionalLightSystem> m_DirectionalLightSystem;

		std::shared_ptr<GUISystem> m_GUISystem;

		std::shared_ptr<CameraSystem> m_CameraSystem;

		std::shared_ptr<CollisionSystem> m_CollisionSystem;
		std::shared_ptr<PhysicsSystem> m_PhysicsSystem;

		std::shared_ptr<TilemapSystem> m_TilemapSystem;
		std::shared_ptr<BoxColliderDebugSystem> m_BoxColliderDebugSystem;

		std::shared_ptr<AnimationSystem> m_AnimationSystem;


		bool m_DisplayDebug = true;

	public:
		static Application* GetApplicationHandle() { return s_Instance; }
		static sf::Vector2u GetWindowSize() { return GetApplicationHandle()->GetWindowDimensions(); }

	private:
		static Application* s_Instance;
	};

	// to be defined by the user
	Application* CreateApplication();
}
