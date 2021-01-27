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

		void Start()
		{
			for (auto const& entity : m_Entities)
			{
				auto& scriptComponent = m_Coordinator->GetComponent<NativeScripts>(entity);
				for (ScriptableEntity& script : scriptComponent.Scripts)
					script.Start();
			}
		}

		void Update(float timestep)
		{
			for (auto const& entity : m_Entities)
			{
				auto& scriptComponent = m_Coordinator->GetComponent<NativeScripts>(entity);
				for (ScriptableEntity& script : scriptComponent.Scripts)
					script.Update(timestep);
			}
		}

	private:
		Coordinator* m_Coordinator;
	};

}
