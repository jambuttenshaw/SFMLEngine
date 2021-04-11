#pragma once


#include "SFMLEngine/ECS/Coordinator.h"


namespace SFMLEngine {

	class Transform;
	class BoxCollider;

	class BoxColliderDebugSystem : public System
	{
	public:
		BoxColliderDebugSystem() = default;
		~BoxColliderDebugSystem() = default;

		void Init(Coordinator* coordinator);

		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override;

		void Update();

	private:
		Coordinator* m_Coordinator = nullptr;

		std::unordered_map<Entity, Transform*> m_TransformCache;
		std::unordered_map<Entity, BoxCollider*> m_ColliderCache;
	};

}

