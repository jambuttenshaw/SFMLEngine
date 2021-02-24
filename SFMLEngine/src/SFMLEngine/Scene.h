#pragma once

#include "ECS/Coordinator.h"
#include "Systems/ScriptableEntitySystem.h"
#include "Systems/IdentitySystem.h"


/*
A scene contains the actual content of the game
and is the users interface to the ECS.

The scene will manage its own entities, and pass the relevent information onto the ECS coordinator
to make sure each entity is registered with its relevant systems.


The user should extend this scene class when they want to create their own scene.

Then, in their definition of the virtual Create function,
they can create their entities and attatch their components.
*/


namespace SFMLEngine {

	class Scene
	{
	public:
		Scene();
		~Scene();

		// Scene creation
		void Init(Coordinator* coordinator, std::shared_ptr<ScriptableEntitySystem> scriptableEntitySystem, std::shared_ptr<IdentitySystem> identitySystem);
		void Destroy();

		// pure virtual function to be overridden in the derived class
		// that creates the entities and their components
		virtual void Create() = 0;


		// ENTITY MANAGEMENT
		Entity CreateEntity();
		Entity CreateEntity(const std::string& name, const std::string& tag, const std::string& layerName);
		void DestroyEntity(Entity entity);

		// COMPONENT MANAGEMENT
		template<typename T>
		void AddComponent(Entity entity, const T& component) { m_Coordinator->AddComponent(entity, component); }
		template<typename T>
		void RemoveComponent(Entity entity) { m_Coordinator->RemoveComponent<T>(entity); }
		template<typename T>
		T& GetComponent(Entity entity) { return m_Coordinator->GetComponent<T>(entity); }

		
		// NATIVE SCRIPTING
		template<typename T>
		T& AddNativeScript(Entity entity) { return m_ScriptableEntitySystem->AddNativeScript<T>(entity, this); }
		template<typename T>
		void RemoveNativeScript(Entity entity) { m_ScriptableEntitySystem->RemoveNativeScript<T>(entity); }
		template<typename T>
		T& GetNativeScript(Entity entity) { return m_ScriptableEntitySystem->GetNativeScript<T>(entity); }


		// ENTITY RETRIEVAL
		const std::vector<Entity>& GetEntitiesWithTag(const std::string& tag) { return m_IdentitySystem->GetEntitiesWithTag(tag); }


		void DisplayDebug();

	private:
		Coordinator* m_Coordinator = nullptr;
		std::shared_ptr<ScriptableEntitySystem> m_ScriptableEntitySystem = nullptr;
		std::shared_ptr<IdentitySystem> m_IdentitySystem = nullptr;

		// keeps track of all entities registered in this scene
		std::set<Entity>* m_EntityRegistry;
	};

}
