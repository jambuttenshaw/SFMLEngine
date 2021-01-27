#pragma once

#include "../Constants.h"

#include <set>

namespace SFMLEngine {


	class System
	{
	public:
		virtual void EntityAddedToSystem(Entity entity) = 0;

		std::set<Entity> m_Entities;
	};

}
