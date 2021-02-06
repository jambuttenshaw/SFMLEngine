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
		bool ComponentModified(T component) { return component.m_Modified; }

		std::set<Entity> m_Entities;
	};

}
