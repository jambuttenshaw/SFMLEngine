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
			sf::Vector2f movement = rigidbody.Velocity * (float)ts;

			// apply the movement to the transform
			transform.Position += movement;
			
			// run collision test
			auto const& collisionTest = m_CollisionSystem->TestCollision(entity);

			// if there was a collision we should move the object back and zero its velocity
			if (collisionTest.Collided)
			{
				transform.Position -= movement;
				rigidbody.Velocity = sf::Vector2f(0, 0);
			}
		}
	}

}
