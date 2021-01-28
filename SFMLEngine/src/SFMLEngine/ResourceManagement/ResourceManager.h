#pragma once

#include "../Constants.h"

#include <queue>


namespace SFMLEngine {

	class ResourceManager
	{
	public:
		static void Init();

	private:
		static ResourceID GetNextID();

	private:
		static std::queue<ResourceID> s_AvailableResourceIDs;
		static size_t s_LivingResourceCount;
	};

}
