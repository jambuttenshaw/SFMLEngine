#pragma once

#include "SFMLEngine/Constants.h"
#include "SFMLEngine/Timestep.h"
#include "SFMLEngine/Systems/Physics/CollisionSystem.h"

#include <SFML/System.hpp>


namespace SFMLEngine {

	class Scene;


	/*
	All user-created scripts that interact with the engine should inherit from ScriptableEntity
	Once added to an entity, this subscribes the script to callbacks from the engine.
	*/

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() = default;

		///////////////////////////////
		// These functions are designed to be overriden by the client
		// to subscribe their script to the engines callbacks
		///////////////////////////////

		// Start is called:
		//  - when the game starts
		//  - when the script is added, if the game has already started
		virtual void Start() {}

		// Update is called once per frame
		virtual void Update(Timestep ts) {}

		// Collision callbacks
		// the Collision struct has lots of juicy information about the collision
		virtual void OnColliderEnter(const Collision& collisionData) {}
		virtual void OnColliderStay(const Collision& collisionData) {}
		// Exit callbacks unfortunately have less information
		virtual void OnColliderExit(Entity other) {}

		// the same again, but for Trigger colliders specifically
		virtual void OnTriggerEnter(const Collision& collisionData) {}
		virtual void OnTriggerStay(const Collision& collisionData) {}
		virtual void OnTriggerExit(Entity other) {}

		// this is called when a new scene has been loaded
		virtual void OnSceneLoaded() {}

		///////////////////////////////
		// These functions are NOT to be overriden by the client
		// These are for getting and/or modifying entities' data and components
		///////////////////////////////

		// create a new entity
		Entity CreateEntity();
		// destroy the entity
		void Destroy();

		// components
		template<typename T>
		void AddComponent(T component) { m_SceneHandle->AddComponent(m_EntityHandle, component); }
		template<typename T>
		void RemoveComponent() { m_SceneHandle->RemoveComponent<T>(m_EntityHandle); }
		template<typename T>
		T& GetComponent() { return m_SceneHandle->GetComponent<T>(m_EntityHandle); }

		// components on different entities
		template<typename T>
		void AddComponent(Entity entity, T component) { m_SceneHandle->AddComponent(entity, component); }
		template<typename T>
		void RemoveComponent(Entity entity) { m_SceneHandle->RemoveComponent<T>(entity); }
		template<typename T>
		T& GetComponent(Entity entity) { return m_SceneHandle->GetComponent<T>(entity); }
		

		// native scripting
		template<typename T>
		T& AddNativeScript() { return m_SceneHandle->AddNativeScript<T>(m_EntityHandle); }
		template<typename T>
		void RemoveNativeScript() { m_SceneHandle->RemoveNativeScript<T>(m_EntityHandle); }
		template<typename T>
		T& GetNativeScript() { return m_SceneHandle->GetNativeScript<T>(m_EntityHandle); }

		// native scripts on other entities
		template<typename T>
		T& AddNativeScript(Entity entity) { return m_SceneHandle->AddNativeScript<T>(entity); }
		template<typename T>
		void RemoveNativeScript(Entity entity) { m_SceneHandle->RemoveNativeScript<T>(entity); }
		template<typename T>
		T& GetNativeScript(Entity entity) { return m_SceneHandle->GetNativeScript<T>(entity); }


		// tags
		const std::vector<Entity>& GetEntitiesWithTag(const std::string& tag);

		const std::string& GetEntityName(Entity entity);
		const std::string& GetEntityTag(Entity entity);
		const std::string& GetEntityLayer(Entity entity);


		// sometimes it is useful for the script to know the ID of the entity
		// it is attatched to
		// for example, using physics casts to check if the mouse pointer is intersecting
		// this object
		Entity GetEntityHandle() { return m_EntityHandle; }




		// these are for use by engine only
		// for setting up scriptable entities
		// to allow them to interact with the engine
		void SetSceneHandle(void* scene);
		void SetEntityHandle(Entity entity);

	private:
		// not directly accesible by client scripts
		// the client should not directly interact with the scene
		// or modify the entity handle
		Scene* m_SceneHandle = nullptr;
		Entity m_EntityHandle = INVALID_ENTITY_ID;
	};

}
