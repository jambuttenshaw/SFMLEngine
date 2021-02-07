#pragma once

#include "../Constants.h"

#include <set>

namespace SFMLEngine {


	class System
	{
	public:
		virtual ~System() {};
		virtual void EntityAddedToSystem(Entity entity) = 0;
		virtual void EntityRemovedFromSystem(Entity entity) = 0;

		template<typename T>
		bool ComponentModified(const T& component) const { return component.m_Modified; }
		template<typename T>
		bool ComponentModified(T* component) const { return component->m_Modified; }
		template<typename T>
		void ResetModified(T& component) { component.m_Modified = false; }
		template<typename T>
		void ResetModified(T* component) { component->m_Modified = false; }

		std::set<Entity> m_Entities;
	};

}
