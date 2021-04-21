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
		rigidbody.SetPosition(transform.GetPosition());
		rigidbody.SetOldPosition(transform.GetPosition());

		// create an entry in the collisions map
		// just now it maps to an empty set
		m_Collisions.insert(std::make_pair(entity, std::unordered_map<ColliderID, CollisionCache>{}));
	}

	void PhysicsSystem::EntityRemovedFromSystem(Entity entity)
	{
		// make sure this entity isn't present anywhere in the collisions maps
		if (m_Collisions.find(entity) != m_Collisions.end())
		{
			m_Collisions.erase(entity);
		}

		// also check it isn't the subject of any collisions
		for (auto& pair : m_Collisions)
		{
			std::vector<ColliderID> toRemove;
			for (auto& collider : pair.second)
			{
				if (collider.second.Owner == entity)
				{
					toRemove.push_back(collider.first);
				}
			}
			for (auto& collider : toRemove) pair.second.erase(collider);
		}
	}

	void PhysicsSystem::EntityDeleted(Entity entity)
	{
		// make sure this entity isn't present anywhere in the collisions maps
		if (m_Collisions.find(entity) != m_Collisions.end())
		{
			// no need to send callback to the entity that is just about to be deleted
			m_Collisions.erase(entity);
		}


		// find out if the entity is the subject of any collisions
		for (auto& pair : m_Collisions)
		{
			std::vector<ColliderID> toRemove;
			for (auto& collider : pair.second)
			{
				if (collider.second.Owner == entity)
				{
					// send the collision callback
					CollisionExitCallback(pair.first, collider);
					toRemove.push_back(collider.first);
				}
			}
			for (auto& collider : toRemove) pair.second.erase(collider);
		}
	}

	void PhysicsSystem::Update(float ts)
	{
		ZoneScoped;

		for (auto const& entity : m_Entities)
		{
			ZoneScoped;
			ZoneName("ProcessEntity", 13);

			auto& rigidbody = m_Coordinator->GetComponent<Rigidbody>(entity);
			auto& transform = m_Coordinator->GetComponent<Transform>(entity);

			// add the entities weight onto the resultant force
			rigidbody.ChangeForce(rigidbody.GetMass() * Physics::Gravity);
			rigidbody.SetAcceleration(rigidbody.GetForce() / rigidbody.GetMass());

			rigidbody.ChangeVelocity(rigidbody.GetAcceleration() * (float)ts);
			rigidbody.ChangePosition(rigidbody.GetVelocity() * (float)ts);


			// once the final movement has been calculated, reset the force to 0
			// each force only lasts for one frame, to apply a bigger impulse the force should be reapplied for many frames
			rigidbody.SetForce({ 0.0f, 0.0f });

			// the final amount the entity will move this frame
			transform.Translate(rigidbody.GetPosition() - rigidbody.GetOldPosition());


			// test for collisions against each collider in the same or adjacent (if required) partitions
			auto& colliders = m_CollisionSystem->GetAllCollidersSharingPartition(entity);
			for (auto const& collider : colliders)
			{
				// dont check collision against itself
				if (collider->Owner == entity) continue;

				// test against the current collider
				Collision collisionTest = m_CollisionSystem->TestCollision(entity, collider);

				// did a collision occur?
				if (collisionTest.OtherColliderID != NULL_COLLIDER_ID)
				{
					// did we collide with a trigger?
					// if we did, we only want to send callbacks
					// we do not want to adjust physics
					if (!collisionTest.Trigger)
					{

						// there was a collision so we should move the object back and zero its velocity

						// the offset is the difference between the transforms position and the colliders position
						// when we are correcting the position of the entity, we are adjusting the transform's position
						// so we need to know the difference between the colliders pos and the transforms pos to adjust properly
						sf::Vector2f offset = transform.GetPosition() - sf::Vector2f{ collisionTest.GlobalBounds.left, collisionTest.GlobalBounds.top };
						
						
						sf::Vector2f newPos{ transform.GetPosition() };

						// respond to the collision based off of the direction
						switch (collisionTest.CollisionDirection)
						{
						case Math::Direction::Right:
							// set the right of this entity to the left of the other entity
							newPos.x = collisionTest.OtherGlobalBounds.left - collisionTest.GlobalBounds.width + offset.x;
							rigidbody.SetVelocity({ 0.0f, rigidbody.GetVelocity().y });
							break;
						case Math::Direction::Left:
							// set the left of this entity to the right of the other entity
							newPos.x = collisionTest.OtherGlobalBounds.left + collisionTest.OtherGlobalBounds.width + offset.x;
							rigidbody.SetVelocity({ 0.0f, rigidbody.GetVelocity().y });
							break;
						case Math::Direction::Down:
							// set the bottom of this entity to the top of the other entity
							newPos.y = collisionTest.OtherGlobalBounds.top - collisionTest.GlobalBounds.height + offset.y;
							rigidbody.SetVelocity({ rigidbody.GetVelocity().x, 0.0f });
							break;
						case Math::Direction::Up:
							// set the top of this entity to the bottom of the other entity
							newPos.y = collisionTest.OtherGlobalBounds.top + collisionTest.OtherGlobalBounds.height + offset.y;
							rigidbody.SetVelocity({ rigidbody.GetVelocity().x, 0.0f });
							break;
						}
						transform.SetPosition(newPos);
					}


					// send collision callback to entities
					CollisionEnterCallback(entity, collisionTest);
				}
			}

			// check to see if we have exited any colliders this frame
			std::set<ColliderID> toRemove;
			for (auto const& collision : m_Collisions[entity])
			{
				if (m_ThisFrameCollisions.find(collision.first) == m_ThisFrameCollisions.end())
				{
					// we have previously logged this collision, but it did not occur again this frame
					// we must have exited the collider
					// send callback
					CollisionExitCallback(entity, collision);
					toRemove.insert(collision.first);
				}
			}
			for (ColliderID c : toRemove) m_Collisions[entity].erase(c);
			m_ThisFrameCollisions.clear();


			// apply any collision detection changes to the rigidbody
			rigidbody.SetPosition(transform.GetPosition());
			rigidbody.SetOldPosition(transform.GetPosition());
		}
	}

	void PhysicsSystem::CollisionEnterCallback(Entity entity, const Collision& collisionData)
	{
		ZoneScoped;

		m_ThisFrameCollisions.insert(collisionData.OtherColliderID);

		// all entities registered in the physics system have entries in the collisions map
		// check that this entity has not already registered a collision with this other entity
		auto& entry = m_Collisions[entity];
		if (entry.find(collisionData.OtherColliderID) == entry.end())
		{
			// insert this collision into the set
			entry.insert(std::make_pair(collisionData.OtherColliderID, CollisionCache{ collisionData.Other, collisionData.Trigger }));


			// run collision callback
			if (m_Coordinator->HasComponent<NativeScripts>(entity))
			{
				auto& scriptsComponent = m_Coordinator->GetComponent<NativeScripts>(entity);
				for (auto& script : scriptsComponent.GetScripts())
				{
					if (collisionData.Trigger)
						script.second->OnTriggerEnter(collisionData);
					else
						script.second->OnColliderEnter(collisionData);
				}
			}
		}
		else
		{
			// we've already sent a collision enter callback, so instead send a collision stay callback
			// run collision callback
			if (m_Coordinator->HasComponent<NativeScripts>(entity))
			{
				auto& scriptsComponent = m_Coordinator->GetComponent<NativeScripts>(entity);
				for (auto& script : scriptsComponent.GetScripts())
				{
					if (collisionData.Trigger)
						script.second->OnTriggerStay(collisionData);
					else
						script.second->OnColliderStay(collisionData);
				}
			}
		}
	}

	void PhysicsSystem::CollisionExitCallback(Entity entity, const std::pair<ColliderID, CollisionCache>& collision)
	{
		ZoneScoped;

		// send a collision exit callback for every registered collision

		// run collision callback
		if (m_Coordinator->HasComponent<NativeScripts>(entity))
		{
			auto& scriptsComponent = m_Coordinator->GetComponent<NativeScripts>(entity);
			for (auto& script : scriptsComponent.GetScripts())
			{
				if (collision.second.Trigger)
					script.second->OnTriggerExit({ collision.second.Owner, collision.first });
				else
					script.second->OnColliderExit({ collision.second.Owner, collision.first });
			}
		}
	}
	
}
