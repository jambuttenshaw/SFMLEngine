#pragma once


#include "../ECS/Coordinator.h"
#include "../ECS/Components/Rigidbody.h"
#include "../ECS/Components/Transform.h"

#include "CollisionSystem.h"

#include "../Timestep.h"


namespace SFMLEngine {

	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem() : m_Gravity(0, 100) {}
		~PhysicsSystem() = default;

		void Init(Coordinator* coordinator, std::shared_ptr<CollisionSystem> collisionSystem);

		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override;

		void Update(Timestep ts);

	private:
		Coordinator* m_Coordinator = nullptr;
		std::shared_ptr<CollisionSystem> m_CollisionSystem;

		sf::Vector2f m_Gravity;
	};

}
