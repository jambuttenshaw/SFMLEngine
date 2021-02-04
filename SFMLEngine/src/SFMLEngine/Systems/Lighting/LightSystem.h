#pragma once

#include "../../ECS/Coordinator.h"
#include "../../ECS/Components.h"


namespace SFMLEngine {

	class LightSystem : public System
	{
	public:
		LightSystem() = default;
		~LightSystem() = default;

		void Init(Coordinator* coordinator)
		{
			m_Coordinator = coordinator;
		}

		void EntityAddedToSystem(Entity entity);
		void EntityRemovedFromSystem(Entity entity);

	protected:
		Coordinator* m_Coordinator = nullptr;

		int m_LightCount = 0;
	};

}

