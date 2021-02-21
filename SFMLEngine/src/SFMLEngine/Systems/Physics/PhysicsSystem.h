#pragma once


#include "SFMLEngine/ECS/Coordinator.h"
#include "SFMLEngine/ECS/Components/Rigidbody.h"
#include "SFMLEngine/ECS/Components/Transform.h"
#include "SFMLEngine/Timestep.h"

#include "CollisionSystem.h"
#include "Physics.h"



namespace SFMLEngine {

	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem() {}
		~PhysicsSystem() = default;

		void Init(Coordinator* coordinator, std::shared_ptr<CollisionSystem> collisionSystem);

		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override;

		void Update(Timestep ts);

		void CollisionEnterCallback(Entity entity, Collision collisionData);
		void CollisionExitCallback(Entity entity);

	private:
		Coordinator* m_Coordinator = nullptr;
		std::shared_ptr<CollisionSystem> m_CollisionSystem;

		std::unordered_map<Entity, std::set<Entity>> m_Collisions;
	};

}
