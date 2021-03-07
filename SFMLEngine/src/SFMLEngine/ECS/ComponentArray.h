#pragma once

#include "SFMLEngine/Constants.h"
#include "SFMLEngine/Core.h"

#include <unordered_map>

namespace SFMLEngine {

	// an interface is required so that the component manager can tell a
	// generic ComponentArray that an entity has been destroyed and it needs
	// to update its component-to-entity mappings
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(Entity entity) = 0;
	};

	// a templated class is required
	// because we will need a different type of component array for each type of component
	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		void InsertData(Entity entity, T component)
		{
			// make sure the entity doesn't already have this component
			SFMLE_CORE_ASSERT(m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end(), "Component added to same entity more than once.");

			size_t newIndex = m_Size;

			// insert the mapping for this entity
			// the reason we dont just use the entity ID as the index is to attempt to maximise data locality
			// and keep all component data adjacent in memory
			// we have a map to get from index to entity and entity to index
			m_EntityToIndexMap[entity] = newIndex;
			m_IndexToEntityMap[newIndex] = entity;

			// add the component to the array
			m_ComponentArray[newIndex] = component;

			++m_Size;
		}

		void RemoveData(Entity entity)
		{
			// make sure the component exists
			SFMLE_CORE_ASSERT(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end(), "Removing non-existent component.");

			// copy element at end into deleted elements place to maintain tightly packed array
			size_t indexOfRemovedEntity = m_EntityToIndexMap[entity];
			size_t indexOfLastElement = m_Size - 1;

			// preserve data density by moving the entity in the last spot of the array
			// to the hole left by the new entity

			// this is done by moving the component data and then updating the index mappings
			// then removing the last in the mappings

			// use move semantics as we dont need to access the data through its old location anymore
			m_ComponentArray[indexOfRemovedEntity] = std::move(m_ComponentArray[indexOfLastElement]);

			// update mappings
			Entity entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
			m_EntityToIndexMap[entityOfLastElement] = indexOfLastElement;
			m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

			// erase the redundant data
			m_EntityToIndexMap.erase(entity);
			m_IndexToEntityMap.erase(indexOfLastElement);

			--m_Size;
		}

		T& GetData(Entity entity)
		{
			// make sure this component exists
			SFMLE_CORE_ASSERT(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end(), "Retrieving non-existent component.");

			// return a reference to the entity's component
			return m_ComponentArray[m_EntityToIndexMap[entity]];
		}

		void EntityDestroyed(Entity entity) override
		{
			// dont assert here, because its possible that we would be requested to remove an entities data that doesnt have data for this component
			if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end())
			{
				// Remove the entity's component if it existed
				RemoveData(entity);
			}
		}

	private:
		// each entity has a unique spot in the array
		// but the array will be packed tightly to improve efficiency
		std::array<T, MAX_ENTITIES> m_ComponentArray;

		// map from an entity id to an array index
		std::unordered_map<Entity, size_t> m_EntityToIndexMap;

		// map from an array index to an entity id
		std::unordered_map<size_t, Entity> m_IndexToEntityMap;


		// total size of valid entries in the array
		size_t m_Size = 0;
	};

}
