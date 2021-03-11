#include "PhysicsSystem.h"

#include "SFMLEngine/ECS/Components/NativeScripts.h"
#include "SFMLEngine/Math.h"
#include "SFMLEngine/DebugTools.h"

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
			transform.Position += rigidbody.Position - rigidbody.OldPosition;


			// run collision test
			auto const& allCollisions = m_CollisionSystem->TestCollision(entity);

			for (auto const& collisionTest : allCollisions)
			{
				// there was a collision so we should move the object back and zero its velocity

				// the offset is the difference between the transforms position and the colliders position
				// when we are correcting the position of the entity, we are adjusting the transform's position
				// so we need to know the difference between the colliders pos and the transforms pos to adjust properly
				sf::Vector2f offset = transform.Position - sf::Vector2f{ collisionTest.GlobalBounds.left, collisionTest.GlobalBounds.top };

				// respond to the collision based off of the direction
				switch (collisionTest.CollisionDirection)
				{
				case Math::Direction::Right:
					// set the right of this entity to the left of the other entity
					transform.Position.x = collisionTest.OtherGlobalBounds.left - collisionTest.GlobalBounds.width + offset.x;
					rigidbody.Velocity.x = 0;
					break;
				case Math::Direction::Left:
					// set the left of this entity to the right of the other entity
					transform.Position.x = collisionTest.OtherGlobalBounds.left + collisionTest.OtherGlobalBounds.width + offset.x;
					rigidbody.Velocity.x = 0;
					break;
				case Math::Direction::Down:
					// set the bottom of this entity to the top of the other entity
					transform.Position.y = collisionTest.OtherGlobalBounds.top - collisionTest.GlobalBounds.height + offset.y;
					rigidbody.Velocity.y = 0;
					break;
				case Math::Direction::Up:
					// set the top of this entity to the bottom of the other entity
					transform.Position.y = collisionTest.OtherGlobalBounds.top + collisionTest.OtherGlobalBounds.height + offset.y;
					rigidbody.Velocity.y = 0;
					break;
				}

				// send collision callback to entities
				CollisionEnterCallback(entity, collisionTest);
				
			}

			if (!allCollisions.size())
			{
				CollisionExitCallback(entity);
			}


			// apply any collision detection changes to the rigidbody
			rigidbody.Position = transform.Position;
			rigidbody.OldPosition = transform.Position;
		}
	}

	void PhysicsSystem::CollisionEnterCallback(Entity entity, Collision collisionData)
	{
		ZoneScoped;

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
		ZoneScoped;

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
