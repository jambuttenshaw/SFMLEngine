#pragma once


#include "SFMLEngine/ECS/Coordinator.h"
#include "SFMLEngine/ECS/Components/Rigidbody.h"
#include "SFMLEngine/ECS/Components/Transform.h"
#include "SFMLEngine/Timestep.h"

#include "CollisionSystem.h"



namespace SFMLEngine {

	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem() : m_Gravity(0, 750) {}
		~PhysicsSystem() = default;

		void Init(Coordinator* coordinator, std::shared_ptr<CollisionSystem> collisionSystem);

		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override;

		void Update(Timestep ts);

		void CollisionCallback(Entity entity, Collision collisionData);

	private:
		Coordinator* m_Coordinator = nullptr;
		std::shared_ptr<CollisionSystem> m_CollisionSystem;

		sf::Vector2f m_Gravity;
	};

}
