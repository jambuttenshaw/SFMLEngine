#pragma once

#include "../ECS/Coordinator.h"
#include "../ECS/Components.h"
#include "../ECS/System.h"

namespace SFMLEngine {

	class ScriptableEntitySystem : public System
	{
	public:
		ScriptableEntitySystem() = default;

		void Init(Coordinator* coordinator) 
		{
			m_Coordinator = coordinator;
		}

		void EntityAddedToSystem(Entity entity) override {}

		void Start()
		{
			for (auto const& entity : m_Entities)
			{
				auto& scriptComponent = m_Coordinator->GetComponent<NativeScripts>(entity);
				for (ScriptableEntity* script : scriptComponent.Scripts)
					script->Start();
			}
		}

		void Update(float timestep)
		{
			for (auto const& entity : m_Entities)
			{
				auto& scriptComponent = m_Coordinator->GetComponent<NativeScripts>(entity);
				for (ScriptableEntity* script : scriptComponent.Scripts)
					script->Update(timestep);
			}
		}

		template<typename T>
		T& AddNativeScript(Entity entity)
		{
			T newScript = new T;
			// check if the entity has a native scripts component
			if (m_Coordinator->HasComponent<NativeScripts>(entity))
			{
				// it already has a native script component
				// we just need to add this new script to the vector of scripts attatched to this component
				auto const& nativeScriptComponent = m_Coordinator->GetComponent<NativeScripts>(entity);
				nativeScriptComponent.Scripts.push_back(newScript);
			}
			else
			{
				auto const& nativeScriptComponent = m_Coordinator->AddComponent(entity, NativeScripts{});
				nativeScriptComponent.Scripts.push_back(newScript);
			}

			return *newScript;
		}

	private:
		Coordinator* m_Coordinator;
	};

}
