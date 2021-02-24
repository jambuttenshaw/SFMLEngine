#pragma once

#include "SFMLEngine/ECS/Coordinator.h"
#include "SFMLEngine/ECS/Components/NativeScripts.h"
#include "SFMLEngine/ECS/System.h"
#include "SFMLEngine/Timestep.h"

#include <type_traits>


namespace SFMLEngine {

	class ScriptableEntitySystem : public System
	{
	public:
		ScriptableEntitySystem() = default;
		
		~ScriptableEntitySystem()
		{
			// the scriptable entity system is responsible for deleting the
			// instances of the scripts that have been created
			for (auto const& entity : m_Entities)
			{
				auto& nativeScriptComponent = m_Coordinator->GetComponent<NativeScripts>(entity);
				for (auto script : nativeScriptComponent.Scripts)
				{
					delete script.second;
				}
				nativeScriptComponent.Scripts.clear();
			}
		}

		void Init(Coordinator* coordinator) 
		{
			m_Coordinator = coordinator;
		}

		void EntityAddedToSystem(Entity entity) override {}
		void EntityRemovedFromSystem(Entity entity) override {}

		void Start()
		{
			for (auto const& entity : m_Entities)
			{
				auto& scriptComponent = m_Coordinator->GetComponent<NativeScripts>(entity);
				for (auto script : scriptComponent.Scripts)
					script.second->Start();
			}
		}

		void Update(Timestep ts)
		{
			ZoneScoped;

			for (auto const& entity : m_Entities)
			{
				auto& scriptComponent = m_Coordinator->GetComponent<NativeScripts>(entity);
				for (auto script : scriptComponent.Scripts)
					script.second->Update(ts);
			}
		}

		template<typename T>
		T& AddNativeScript(Entity entity, void* scene)
		{
			static_assert(std::is_base_of<ScriptableEntity, T>::value, "Native scripts must inherit from Scriptable entity!");
			
			const char* typeName = typeid(T).name();

			T* newScript = new T;

			ScriptableEntity* scriptableEntity = dynamic_cast<ScriptableEntity*>(newScript);
			scriptableEntity->SetSceneHandle(scene);
			scriptableEntity->SetEntityHandle(entity);

			// check if the entity has a native scripts component
			if (!m_Coordinator->HasComponent<NativeScripts>(entity))
				m_Coordinator->AddComponent(entity, NativeScripts{});

			auto& nativeScriptComponent = m_Coordinator->GetComponent<NativeScripts>(entity);
			nativeScriptComponent.Scripts.insert({ {typeName, scriptableEntity} });

			return *newScript;
		}

		template<typename T>
		void RemoveNativeScript(Entity entity)
		{
			const char* typeName = typeid(T).name();
			auto& scripts = m_Coordinator->GetComponent<NativeScripts>(entity).Scripts;

			assert(scripts.find(typeName) != scripts.end() && "Entity did not have script attatched!");

			scripts.erase(typeName);
		}

		template<typename T>
		T& GetNativeScript(Entity entity)
		{
			const char* typeName = typeid(T).name();
			auto& scripts = m_Coordinator->GetComponent<NativeScripts>(entity).Scripts;

			auto location = scripts.find(typeName);

			assert(location != scripts.end() && "Entity did not have script attatched!");

			return *static_cast<T*>(scripts.at(typeName));
		}

	private:
		Coordinator* m_Coordinator = nullptr;
	};

}
