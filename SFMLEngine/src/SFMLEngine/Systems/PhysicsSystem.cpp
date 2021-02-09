#include "PhysicsSystem.h"


namespace SFMLEngine {


	void PhysicsSystem::Init(Coordinator* coordinator)
	{
		m_Coordinator = coordinator;
	}

	void PhysicsSystem::EntityAddedToSystem(Entity entity)
	{

	}

	void PhysicsSystem::EntityRemovedFromSystem(Entity entity)
	{

	}

	void PhysicsSystem::Update(Timestep ts)
	{
		for (auto const& entity : m_Entities)
		{
			auto& rigidbody = m_Coordinator->GetComponent<Rigidbody>(entity);
			auto& transform = m_Coordinator->GetComponent<Transform>(entity);

			transform.Position += rigidbody.Velocity * (float)ts;
			rigidbody.Velocity += m_Gravity * (float)ts;
		}
	}

}
