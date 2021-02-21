#pragma once

#include "SFMLEngine/Constants.h"
#include "SFMLEngine/Timestep.h"
#include "SFMLEngine/Systems/Physics/CollisionSystem.h"

#include <SFML/System.hpp>


namespace SFMLEngine {

	class Scene;

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {};

		virtual void Start() {}

		virtual void Update(Timestep ts) {}

		virtual void OnCollisionEnter(Collision collisionData) {}
		virtual void OnCollisionExit(Entity other) {}

		// components
		template<typename T>
		T& AddComponent(T component) { return m_SceneHandle->AddComponent(m_EntityHandle, component); }
		template<typename T>
		void RemoveComponent() { m_SceneHandle->RemoveComponent<T>(m_EntityHandle); }
		template<typename T>
		T& GetComponent() { return m_SceneHandle->GetComponent<T>(m_EntityHandle); }

		// components on different entities
		template<typename T>
		T& GetComponent(Entity entity) { return m_SceneHandle->GetComponent<T>(entity); }

		// native scripting
		template<typename T>
		T& AddNativeScript() { return m_SceneHandle->AddNativeScript<T>(m_EntityHandle); }
		template<typename T>
		void RemoveNativeScript() { m_SceneHandle->RemoveNativeScript<T>(m_EntityHandle); }
		template<typename T>
		T& GetNativeScript() { return m_SceneHandle->GetNativeScript<T>(m_EntityHandle); }

		const std::vector<Entity>& GetEntitiesWithTag(const std::string& tag);

		void SetSceneHandle(void* scene);
		void SetEntityHandle(Entity entity);

	private:
		Scene* m_SceneHandle = nullptr;
		Entity m_EntityHandle = INVALID_ENTITY_ID;
	};

}
