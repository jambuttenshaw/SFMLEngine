#pragma once

#include "SFMLEngine/Constants.h"
#include "SFMLEngine/Core.h"

#include <unordered_map>
#include <set>

namespace SFMLEngine {

	// an interface is required so that the component manager can contain an
	// array of generic ComponentArray objects
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(Entity entity) = 0;
	};


	// a templated class is required
	// because we will need a different type of component array for each type of component

	// a component array is pretty much a wrapper on std::array, with additional validation when adding and removing components
	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		void InsertData(Entity entity, T component)
		{
			SFMLE_CORE_ASSERT(m_PopulatedEntities.find(entity) == m_PopulatedEntities.end(), "Entity already has this component.");

			// insert the given data at the entity's place in the array
			m_ComponentArray[entity] = component;

			// mark this entity as containing the component
			m_PopulatedEntities.insert(entity);
		}

		void RemoveData(Entity entity)
		{
			// mark that this entity no longer has this component
			m_PopulatedEntities.erase(entity);
		}

		T& GetData(Entity entity)
		{
			// make sure this component exists
			SFMLE_CORE_ASSERT(m_PopulatedEntities.find(entity) != m_PopulatedEntities.end(), "Retrieving non-existent component.");

			// return a reference to the entity's component
			return m_ComponentArray[entity];
		}

		void EntityDestroyed(Entity entity) override
		{
			// dont assert here, because its possible that we would be requested to remove an entities data that doesnt have data for this component
			if (m_PopulatedEntities.find(entity) != m_PopulatedEntities.end())
			{
				RemoveData(entity);
			}
		}

	private:
		// each entity has a unique spot in the array
		std::array<T, MAX_ENTITIES> m_ComponentArray;

		// keep track of which entities have this component already
		// to avoid adding the same component multiple times
		// and trying to retrieve non-existant components
		std::set<Entity> m_PopulatedEntities;
	};


}
