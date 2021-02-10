#include "PhysicsSystem.h"


namespace SFMLEngine {


	void PhysicsSystem::Init(Coordinator* coordinator, std::shared_ptr<CollisionSystem> collisionSystem)
	{
		m_Coordinator = coordinator;
		m_CollisionSystem = collisionSystem;
	}

	void PhysicsSystem::EntityAddedToSystem(Entity entity)
	{

	}

	void PhysicsSystem::EntityRemovedFromSystem(Entity entity)
	{

	}

	void PhysicsSystem::Update(Timestep ts)
	{
		ZoneScoped;

		for (auto const& entity : m_Entities)
		{
			auto& rigidbody = m_Coordinator->GetComponent<Rigidbody>(entity);
			auto& transform = m_Coordinator->GetComponent<Transform>(entity);

			rigidbody.Velocity += m_Gravity * (float)ts;
			transform.Position += rigidbody.Velocity * (float)ts;
		}
	}

}
