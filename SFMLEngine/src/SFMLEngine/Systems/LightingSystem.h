#pragma once

#include "../ECS/Coordinator.h"
#include "../ECS/Components.h"

namespace SFMLEngine {

	class LightingSystem : public System
	{
	public:
		LightingSystem() = default;
		~LightingSystem() = default;

		void Init(Coordinator* coordinator)
		{
			m_Coordinator = coordinator;
		}

		void EntityAddedToSystem(Entity entity) override {};

	private:
		Coordinator* m_Coordinator = nullptr;
	};

}
