#include "PhysicsSystem.h"

#include "SFMLEngine/ECS/Components/NativeScripts.h"


namespace SFMLEngine {


	void PhysicsSystem::Init(Coordinator* coordinator, std::shared_ptr<CollisionSystem> collisionSystem)
	{
		m_Coordinator = coordinator;
		m_CollisionSystem = collisionSystem;
	}

	void PhysicsSystem::EntityAddedToSystem(Entity entity)
	{
		auto& rigidbody = m_Coordinator->GetComponent<Rigidbody>(entity);
		auto& transform = m_Coordinator->GetComponent<Transform>(entity);

		// set up the rigidbody
		rigidbody.Position = transform.Position;
		rigidbody.OldPosition = transform.Position;

		// create an entry in the collisions map
		// just now it maps to an empty set
		m_Collisions.insert(std::make_pair(entity, std::set<Entity>{}));
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

			// add the entities weight onto the resultant force
			rigidbody.Force += rigidbody.Mass * Physics::Gravity;
			rigidbody.Acceleration = rigidbody.Force / rigidbody.Mass;

			rigidbody.Velocity += rigidbody.Acceleration * (float)ts;
			rigidbody.Position += rigidbody.Velocity * (float)ts;


			// once the final movement has been calculated, reset the force to 0
			// each force only lasts for one frame, to apply a bigger impulse the force should be reapplied for many frames
			rigidbody.Force *= 0.0f;

			// the final amount the entity will move this frame
			sf::Vector2f movement = rigidbody.Position - rigidbody.OldPosition;

			
			// detect if collisions occured on any axis
			bool anyCollisions = false;

			// deal with movement component-wise so we can detect collisions on each axis seperately

			{
				// x axis
				// apply the movement to the transform
				transform.Position.x += movement.x;


				// run collision test
				auto const& collisionTest = m_CollisionSystem->TestCollision(entity);

				// if there was a collision we should move the object back and zero its velocity
				if (collisionTest.Collided)
				{
					float offset = transform.Position.x - collisionTest.GlobalBounds.left;
					if (movement.x > 0)
					{
						// moving right: set the right of the collider to the left of the other collider
						transform.Position.x = collisionTest.OtherGlobalBounds.left - collisionTest.GlobalBounds.width + offset;
					}
					else
					{
						// moving left: set the left of the collider to the right of the other collider
						transform.Position.x = collisionTest.OtherGlobalBounds.left + collisionTest.OtherGlobalBounds.width + offset;
					}
					rigidbody.Velocity.x = 0;

					CollisionEnterCallback(entity, collisionTest);
					anyCollisions = true;
				}
			}

			{
				// y axis
				// apply the movement to the transform
				transform.Position.y += movement.y;

				// run collision test
				auto const& collisionTest = m_CollisionSystem->TestCollision(entity);

				// if there was a collision we should move the object back and zero its velocity
				if (collisionTest.Collided)
				{
					float offset = transform.Position.y - collisionTest.GlobalBounds.top;
					if (movement.y > 0)
					{
						// moving down: set the bottom of the collider to the top of the other collider
						transform.Position.y = collisionTest.OtherGlobalBounds.top - collisionTest.GlobalBounds.height + offset;
					}
					else
					{
						// moving up: set the top of the collider to the bottom of the other collider
						transform.Position.y = collisionTest.OtherGlobalBounds.top + collisionTest.OtherGlobalBounds.height + offset;
					}
					rigidbody.Velocity.y = 0;

					CollisionEnterCallback(entity, collisionTest);
					anyCollisions = true;
				}
			}

			if (!anyCollisions)
			{
				// no collisions happened with this entity this frame
				// collision exit callback
				CollisionExitCallback(entity);
			}

			// apply any collision detection changes to the rigidbody
			rigidbody.Position = transform.Position;
			rigidbody.OldPosition = rigidbody.Position;
		}
	}

	void PhysicsSystem::CollisionEnterCallback(Entity entity, Collision collisionData)
	{
		// all entities registered in the physics system have entries in the collisions map
		// check that this entity has not already registered a collision with this other entity
		auto& entry = m_Collisions[entity];
		if (entry.find(collisionData.Other) == entry.end())
		{
			// insert this collision into the set
			entry.insert(collisionData.Other);

			// run collision callback
			if (m_Coordinator->HasComponent<NativeScripts>(entity))
			{
				auto& scriptsComponent = m_Coordinator->GetComponent<NativeScripts>(entity);
				for (auto& script : scriptsComponent.Scripts)
				{
					script.second->OnCollisionEnter(collisionData);
				}
			}
		}
	}

	void PhysicsSystem::CollisionExitCallback(Entity entity)
	{
		// send a collision exit callback for every registered collision
		auto& entry = m_Collisions[entity];
		if (entry.size() > 0)
		{
			// run collision callback
			if (m_Coordinator->HasComponent<NativeScripts>(entity))
			{
				auto& scriptsComponent = m_Coordinator->GetComponent<NativeScripts>(entity);
				for (auto& script : scriptsComponent.Scripts)
				{
					for (Entity other : entry)
						script.second->OnCollisionExit(other);
				}
			}

			entry.clear();
		}
	}

}
