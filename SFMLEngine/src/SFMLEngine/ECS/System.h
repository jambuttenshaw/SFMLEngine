#pragma once

#include "SFMLEngine/Constants.h"

#include <set>

namespace SFMLEngine {

	/*
	An interface that defines a system that interacts with the ECS
	*/
	class System
	{
	public:
		virtual ~System() {};
		// systems can respond to when entities are added and removed from the system
		virtual void EntityAddedToSystem(Entity entity) = 0;
		virtual void EntityRemovedFromSystem(Entity entity) = 0;

		// systems are also able to find out if a component has been modified
		// and are able to reset the components modified flag
		template<typename T>
		bool ComponentModified(const T& component) const { return component.m_Modified; }
		template<typename T>
		bool ComponentModified(T* component) const { return component->m_Modified; }
		template<typename T>
		void ResetModified(T& component) { component.m_Modified = false; }
		template<typename T>
		void ResetModified(T* component) { component->m_Modified = false; }

		// a system has a collection of entities
		std::set<Entity> m_Entities;
	};

}
