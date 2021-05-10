#pragma once

#include "SFMLEngine/Constants.h"
#include "ComponentArray.h"
#include "SFMLEngine/Core.h"

#include <unordered_map>
#include <memory>

namespace SFMLEngine {


	class ComponentManager
	{
	public:
		template<typename T>
		void RegisterComponent()
		{
			// the type name of the component is used as its unique identifier in all of the mappings
			const char* typeName = typeid(T).name();

			// make sure the component hasnt been registered already
			SFMLE_CORE_ASSERT(m_ComponentTypes.find(typeName) == m_ComponentTypes.end(), "Registering component type more than once.");

			// add this component type to the component type map
			m_ComponentTypes.insert({ typeName, m_NextComponentType });

			// create a component array pointer and add it to the component arrays map
			m_ComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

			// increment the value so the next component registered will be different
			++m_NextComponentType;
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			// the component type is the index for the bit in the signature that represents this component
			// ie if an entity has a signature 0100 and this components type is 2, then that entity has this component

			// get the unique identifier for this component
			const char* typeName = typeid(T).name();

			// make sure that the component exists
			SFMLE_CORE_ASSERT(m_ComponentTypes.find(typeName) != m_ComponentTypes.end(), "Component not registered before use.");

			// return the components type: used for creating signatures
			return m_ComponentTypes[typeName];
		}

		template<typename T>
		void AddComponent(Entity entity, const T& component)
		{
			// adds component data for this entity
			// handled by the component array
			GetComponentArray<T>()->InsertData(entity, component);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			// removes the data associated with this entity
			GetComponentArray<T>()->RemoveData(entity);
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			// returns the data for this component associated with this entity
			return GetComponentArray<T>()->GetData(entity);
		}

		void EntityDestroyed(Entity entity)
		{
			// notify each component that an entity has been destroyed
			// if it has a component for that entity it will remove it
			for (auto const& pair : m_ComponentArrays)
			{
				auto const& component = pair.second;
				component->EntityDestroyed(entity);
			}
		}

	private:

		// map from type string pointer to a component type
		std::unordered_map<const char*, ComponentType> m_ComponentTypes{};
		// map from type string pointer to a component array
		std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_ComponentArrays{};

		// the component type to be assigned to the next registered component
		ComponentType m_NextComponentType{};

		// convenience function to get the statically casted pointer to the ComponentArray
		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			// get the identifier of this component
			const char* typeName = typeid(T).name();

			// make sure this component exists
			assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registered before use.");

			// static_pointer_cast is required here as component arrays are captured in shared_ptr's
			return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
		}

	};

}
