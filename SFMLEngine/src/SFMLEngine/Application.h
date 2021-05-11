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

#include "Systems/GUI/GUIPositionSystem.h"
#include "Systems/GUI/GUIImageSystem.h"
#include "Systems/GUI/GUITextSystem.h"

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

		void Close() { m_Window->close(); }

		void Shutdown();

		void SetWindowIcon(const std::string& iconPath);
		sf::RenderWindow* GetWindowHandle() { return m_Window; }


		void SetVSync(bool state);
		inline bool IsVSync() { return m_VSync; }

		void SetFPSLimit(unsigned int limit);
		unsigned int GetFPSLimit() { return m_FPSLimit; }

		inline float GetTimeScale() const { return m_TimeScale; }
		inline void SetTimeScale(float scale) { m_TimeScale = scale; }

		void SetClearColor(const sf::Color& color) { m_ClearColor = color; }
		const sf::Color& GetClearColor() { return m_ClearColor; }

		void DisplayDebugInfo(bool state) { m_DisplayDebug = state; }


		template <typename T>
		void LoadScene(LoadSceneMode mode)
		{
			// T MUST inherit from Scene
			static_assert(std::is_base_of<Scene, T>::value, "T should inherit from Scene.");

			// create a new scene
			T* newScene = new T;

			// check if theres a scene already loaded
			// in which case scene loading must wait until the start of the next frame
			if (m_CurrentScenes.size())
			{
				m_ScenesToLoad.push_back(std::make_pair(static_cast<Scene*>(newScene), mode));
			}
			else
			{
				// no scene currently exists, so we dont need to delete the old one first
				// that means scene loading can be completed this frame

				// also, it does not matter the loading mode as both would produce the same result

				// even though no scene was already loaded, reset scriptable entity system just to be safe
				m_ScriptableEntitySystem->Reset();

				// implicit conversion to Scene* since T inherits from Scene
				m_CurrentScenes.push_back(newScene);

				newScene->Init(m_Coordinator, m_ScriptableEntitySystem, m_PhysicsSystem, m_IdentitySystem);

				// pure virtual function that will create a scene T
				newScene->Create();
			}
		}

		void DeleteScene(Scene* scene)
		{
			size_t index = 0;
			for (Scene* s : m_CurrentScenes)
			{
				if (s == scene)
				{
					m_ScenesToDelete.push_back(scene);
					return;
				}
				index++;
			}

			LOG_CORE_WARN("Scene was attempted to be deleted, but it was not loaded!");
		}

		const std::vector<Scene*>& GetLoadedScenes() { return m_CurrentScenes; }


	private:
		sf::Vector2u GetWindowDimensions() { return m_Window->getSize(); }

		void DeleteAllCurrentScenes()
		{
			for (Scene* scene : m_CurrentScenes)
			{
				// clear out all of the existing entities
				scene->DestroyThisScene();
				delete scene;
			}
			m_CurrentScenes.clear();
		}
		void CompleteLoadingNewScene(Scene* newScene)
		{
			m_CurrentScenes.push_back(newScene);

			newScene->Init(m_Coordinator, m_ScriptableEntitySystem, m_PhysicsSystem, m_IdentitySystem);

			// pure virtual function that will create a scene T
			newScene->Create();
		}

	private:
		sf::RenderWindow* m_Window;
		// a view with topleft at 0,0 and size = window size
		sf::View m_GUIView;

		sf::Clock m_Clock;

		std::vector<Scene*> m_CurrentScenes;
		std::vector<std::pair<Scene*, LoadSceneMode>> m_ScenesToLoad;
		std::vector<Scene*> m_ScenesToDelete;

		// VSync enabled by default
		bool m_VSync = true;
		unsigned int m_FPSLimit = 0;
		const float m_DeltaTimeLimit = 1.0f / 30.0f;
		float m_TimeScale = 1.0f;

		sf::Color m_ClearColor{ sf::Color::Black };

		ResourceID m_IconImage = NULL_RESOURCE_ID;

		// ECS
		Coordinator* m_Coordinator;

		// ECS systems
		std::shared_ptr<IdentitySystem> m_IdentitySystem;

		std::shared_ptr<SpriteRenderSystem> m_SpriteRenderSystem;
		std::shared_ptr<TilemapRenderSystem> m_TilemapRenderSystem;

		std::shared_ptr<ScriptableEntitySystem> m_ScriptableEntitySystem;

		std::shared_ptr<PointLightSystem> m_PointLightSystem;
		std::shared_ptr<DirectionalLightSystem> m_DirectionalLightSystem;

		std::shared_ptr<GUIPositionSystem> m_GUIPositionSystem;
		std::shared_ptr<GUIImageSystem> m_GUIImageSystem;
		std::shared_ptr<GUITextSystem> m_GUITextSystem;

		std::shared_ptr<CameraSystem> m_CameraSystem;

		std::shared_ptr<CollisionSystem> m_CollisionSystem;
		std::shared_ptr<PhysicsSystem> m_PhysicsSystem;

		std::shared_ptr<TilemapSystem> m_TilemapSystem;
		std::shared_ptr<BoxColliderDebugSystem> m_BoxColliderDebugSystem;

		std::shared_ptr<AnimationSystem> m_AnimationSystem;


		bool m_DisplayDebug = false;

	public:
		static Application* GetApplicationHandle() { return s_Instance; }
		static sf::Vector2u GetWindowSize() { return GetApplicationHandle()->GetWindowDimensions(); }

	private:
		static Application* s_Instance;
	};

	// to be defined by the user
	Application* CreateApplication();
}
