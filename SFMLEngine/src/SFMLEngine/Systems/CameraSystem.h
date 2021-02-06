#pragma once

#include "../ECS/Coordinator.h"
#include "../ECS/Components.h"

namespace SFMLEngine {

	class CameraSystem : public System
	{
	public:
		CameraSystem() = default;
		~CameraSystem() = default;

		void Init(Coordinator* coordinator);

		void EntityAddedToSystem(Entity entity) override {}
		void EntityRemovedFromSystem(Entity entity) override {}

	private:

		Coordinator* m_Coordinator = nullptr;

	};

}
