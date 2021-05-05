#pragma once

#include "ECS/Coordinator.h"
#include "Systems/ScriptableEntitySystem.h"
#include "Systems/Physics/PhysicsSystem.h"
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

	// The LoadSceneMode tells the engine how the scene should be loaded
	// 
	// Single scene loading deletes all existing entities before loading this scenes entities,
	// so it replaces all currently loaded scenes
	// 
	// Additive scene loading adds this scenes entities alongside any entities that may already exist in the scene
	enum class LoadSceneMode
	{
		Single, Additive
	};


	class Scene
	{
	public:
		// ctor and dtor
		// virtual dtor as client scenes are manipulated through common interface
		Scene();
		virtual ~Scene();

		// Scene creation and destruction, only to be used by the application
		// a Scene needs to expose the functionality of certain systems in the engine
		// so it requires to be aware of those systems
		void Init(Coordinator* coordinator,
			std::shared_ptr<ScriptableEntitySystem> scriptableEntitySystem,
			std::shared_ptr<PhysicsSystem> physicsSystem,
			std::shared_ptr<IdentitySystem> identitySystem);

		// Destroys all entities in this scene (and by proxy their components)
		void DestroyThisScene();

		// When an entity is deleted through a script, it does not get deleted immediately
		// to avoid trying to access a deleted entity
		// so all entities get deleted at the beginning of the frame
		// this function deletes all the entities that were queued to be deleted on the previous frame
		void DestroyAllPendingEntities();

		// pure virtual function to be overridden in the derived class
		// that creates the entities and their components
		virtual void Create() = 0;

		///////////////////////
		// ENTITY MANAGEMENT //
		///////////////////////

		// Create a new entity
		Entity CreateEntity();
		// Create a new entity with a name, tag and layer
		Entity CreateEntity(const std::string& name, const std::string& tag, const std::string& layerName);
		// Queue an entity to be deleted at the beginning of the next frame
		void DestroyEntity(Entity entity);
		// Destroy an entity immediately. Warning: Consider using DestroyEntity instead
		void DestroyEntityImmediate(Entity entity);


		//////////////////////////
		// COMPONENT MANAGEMENT //
		//////////////////////////

		// Add a new component
		template<typename T>
		void AddComponent(Entity entity, const T& component) { m_Coordinator->AddComponent(entity, component); }
		// Remove an existing component
		template<typename T>
		void RemoveComponent(Entity entity) { m_Coordinator->RemoveComponent<T>(entity); }
		// Get a reference to a component on an entity
		template<typename T>
		T& GetComponent(Entity entity) { return m_Coordinator->GetComponent<T>(entity); }


		//////////////////////
		// NATIVE SCRIPTING //
		//////////////////////

		// This is just an interface through to the engines ScriptableEntitySystem

		// Add a script to an entity
		template<typename T>
		T& AddNativeScript(Entity entity) { return m_ScriptableEntitySystem->AddNativeScript<T>(entity, this); }
		// Remove a native script from an entity
		template<typename T>
		void RemoveNativeScript(Entity entity) { m_ScriptableEntitySystem->RemoveNativeScript<T>(entity); }
		// Get a reference to a script attatched to an entity
		template<typename T>
		T& GetNativeScript(Entity entity) { return m_ScriptableEntitySystem->GetNativeScript<T>(entity); }


		// Helpful entity identification functions
		// instead of having to interfact with the Entity's Identity component, the scene will do it for you

		// Set an entities name
		void SetEntityName(Entity entity, const std::string& name);
		// Get the name of an entity
		const std::string& GetEntityName(Entity entity);

		// Set an entities tag
		void SetEntityTag(Entity entity, const std::string& tag);
		// Get the tag of an entity
		const std::string& GetEntityTag(Entity entity);

		// Set the layer (used for collision culling) of an entity
		void SetEntityLayer(Entity entity, const std::string& layerName);
		// Get the layer (used for collision culling) of an entity
		const std::string& GetEntityLayer(Entity entity);


		// ENTITY RETRIEVAL

		// Get a vector that contains all of the entities that are associated with the given tag
		const std::vector<Entity>& GetEntitiesWithTag(const std::string& tag) { return m_IdentitySystem->GetEntitiesWithTag(tag); }

	private:
		// deletes all of the data associated with an entity,
		// but does not un-register the entity from the scene
		void DeleteEntityData(Entity entity);

	private:
		// the coordinator for the ECS
		Coordinator* m_Coordinator = nullptr;

		// pointers to the engines systems
		std::shared_ptr<ScriptableEntitySystem> m_ScriptableEntitySystem = nullptr;
		std::shared_ptr<IdentitySystem> m_IdentitySystem = nullptr;
		std::shared_ptr<PhysicsSystem> m_PhysicsSystem = nullptr;

		// keeps track of all entities registered in this scene
		std::set<Entity>* m_EntityRegistry;
		std::vector<Entity> m_EntitiesToDestroy;
	};

}
