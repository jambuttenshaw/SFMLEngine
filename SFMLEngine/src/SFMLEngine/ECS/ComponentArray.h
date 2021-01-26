#pragma once

#include "../Constants.h"

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


	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		void InsertData(Entity entity, T component)
		{
			assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");

			size_t newIndex = m_Size;

			m_EntityToIndexMap[entity] = newIndex;
			m_IndexToEntityMap[newIndex] = entity;

			m_ComponentArray[newIndex] = component;

			++m_Size;
		}

		void RemoveData(Entity entity)
		{
			assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");

			// copy element at end into deleted elements place to maintain tightly packed array
			size_t indexOfRemovedEntity = m_EntityToIndexMap[entity];
			size_t indexOfLastElement = m_Size - 1;

			m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];

			// update map to point to moved spot
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
			assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");

			// return a reference to the entity's component
			return m_ComponentArray[m_EntityToIndexMap[entity]];
		}

		void EntityDestroyed(Entity entity) override
		{
			// dont assert here, because its possible that we would be requested to remove an entities data that doesnt have data for this component
			if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
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
		size_t m_Size;
	};

}
