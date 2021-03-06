#pragma once


#include "SFMLEngine/ECS/Coordinator.h"
#include "SFMLEngine/ECS/Components/Rigidbody.h"
#include "SFMLEngine/ECS/Components/Transform.h"

#include "CollisionSystem.h"
#include "Physics.h"



namespace SFMLEngine {

	class PhysicsSystem : public System
	{
	public:
		struct CollisionCache
		{
			Entity Owner;
			bool Trigger;
		};


		PhysicsSystem() {}
		~PhysicsSystem() = default;

		void Init(Coordinator* coordinator, std::shared_ptr<CollisionSystem> collisionSystem);

		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override;

		void EntityDeleted(Entity entity);

		void Update(float ts);

		void CollisionEnterCallback(Entity entity, const Collision& collisionData);
		void CollisionExitCallback(Entity entity, const std::pair<ColliderID, CollisionCache>& collision);

	private:
		Coordinator* m_Coordinator = nullptr;
		std::shared_ptr<CollisionSystem> m_CollisionSystem;

		std::unordered_map<Entity, std::unordered_map<ColliderID, CollisionCache>> m_Collisions;
		std::set<ColliderID> m_ThisFrameCollisions;
	};

}
