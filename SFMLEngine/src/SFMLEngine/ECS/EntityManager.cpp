#include "EntityManager.h"

#include <cassert>
#include <iostream>

namespace SFMLEngine {


	EntityManager::EntityManager()
	{
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			m_AvailableEntities.push(entity);
		}
	}

	Entity EntityManager::CreateEntity()
	{
		assert(m_LivingEntityCount < MAX_ENTITIES && "Too many entities in existance.");

		// take the entity ID that is at the front of the queue
		Entity id = m_AvailableEntities.front();
		m_AvailableEntities.pop();
		++m_LivingEntityCount;

		return id;
	}

	void EntityManager::DestroyEntity(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		// invalidate the entity-to-destroy's signature
		m_Signatures[entity].reset();

		// place the destroyed entity id at the back of the queue
		m_AvailableEntities.push(entity);
		--m_LivingEntityCount;
	}

	void EntityManager::SetSignature(Entity entity, Signature signature)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		// place this entities signature into the array
		m_Signatures[entity] = signature;
	}

	Signature EntityManager::GetSignature(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		// get this entities signature from the array
		return m_Signatures[entity];
	}


}
