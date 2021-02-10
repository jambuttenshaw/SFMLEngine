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
			ZoneScoped;
			ZoneName("ProcessEntity", 13);

			auto& rigidbody = m_Coordinator->GetComponent<Rigidbody>(entity);
			auto& transform = m_Coordinator->GetComponent<Transform>(entity);

			rigidbody.Velocity += m_Gravity * (float)ts;
			sf::Vector2f movement = rigidbody.Velocity * (float)ts;

			// deal with movement component-wise so we can detect collisions on each axis seperately
			if (fabsf(movement.x) > 0.1f) {
				// x axis
				// apply the movement to the transform
				transform.Position.x += movement.x;
			
			
				// run collision test
				auto const& collisionTest = m_CollisionSystem->TestCollision(entity);

				// if there was a collision we should move the object back and zero its velocity
				if (collisionTest.Collided)
				{
					if (movement.x > 0)
						transform.Position.x = collisionTest.OtherBounds.left - collisionTest.Bounds.width;
					else
						transform.Position.x = collisionTest.OtherBounds.left + collisionTest.OtherBounds.width;
					rigidbody.Velocity.x = 0;
				}
			}

			if (fabsf(movement.y) > 0.1f) {
				// y axis
				// apply the movement to the transform
				transform.Position.y += movement.y;

				// run collision test
				auto const& collisionTest = m_CollisionSystem->TestCollision(entity);

				// if there was a collision we should move the object back and zero its velocity
				if (collisionTest.Collided)
				{
					if (movement.y > 0)
						transform.Position.y = collisionTest.OtherBounds.top - collisionTest.Bounds.height;
					else
						transform.Position.y = collisionTest.OtherBounds.top + collisionTest.OtherBounds.height;
					rigidbody.Velocity.y = 0;
				}
			}
		}
	}

}
