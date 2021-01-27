#pragma once

#include "ECS/Coordinator.h"

namespace SFMLEngine {

	class Scene
	{
	public:
		Scene(Coordinator* coordinator, std::shared_ptr<ScriptableEntitySystem> scriptableEntitySystem)
			: m_Coordinator(coordinator), m_ScriptableEntitySystem(scriptableEntitySystem) {}

		// ENTITY MANAGEMENT
		Entity CreateEntity() { return m_Coordinator->CreateEntity(); }
		void DestroyEntity(Entity entity) { m_Coordinator->DestroyEntity(entity); }

		// COMPONENT MANAGEMENT
		template<typename T>
		T& AddComponent(Entity entity, T component) { return m_Coordinator->AddComponent(entity, component); }
		template<typename T>
		void RemoveComponent(Entity entity) { m_Coordinator->RemoveComponent<T>(entity); }
		template<typename T>
		T& GetComponent(Entity entity) { return m_Coordinator->GetComponent<T>(entity); }

		
		// NATIVE SCRIPTING
		template<typename T>
		T& AddNativeScript(Entity entity) { return m_ScriptableEntitySystem->AddNativeScript<T>(entity); }
		template<typename T>
		void RemoveNativeScript(Entity entity) { m_ScriptableEntitySystem->RemoveNativeScript<T>(entity); }
		template<typename T>
		T& GetNativeScript(Entity entity) { return m_ScriptableEntitySystem->GetNativeScript<T>(entity); }

	private:
		Coordinator* m_Coordinator;
		std::shared_ptr<ScriptableEntitySystem> m_ScriptableEntitySystem;
	};

}
