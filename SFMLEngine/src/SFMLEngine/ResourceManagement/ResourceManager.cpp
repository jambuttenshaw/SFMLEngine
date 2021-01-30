#include "ResourceManager.h"


namespace SFMLEngine {

	// initialize static members
	std::queue<ResourceID> ResourceManager::s_AvailableResourceIDs;
	size_t ResourceManager::s_LivingResourceCount = 0;
	std::unordered_map<ResourceID, ResourceHandle> ResourceManager::s_Resources;

	void ResourceManager::Init()
	{
		for (ResourceID resource = 0; resource < MAX_RESOURCES; ++resource)
		{
			s_AvailableResourceIDs.push(resource);
		}
	}

	void ResourceManager::Shutdown()
	{
		for (auto const& resource : s_Resources)
		{
			LOG_CORE_WARN("Resource ID {0} was not explicitly deleted, type could not be assumed.", resource.first);
			delete resource.second;
		}
		s_Resources.clear();
	}

	ResourceID ResourceManager::GetNextID()
	{
		ResourceID nextID = s_AvailableResourceIDs.front();
		s_AvailableResourceIDs.pop();
		++s_LivingResourceCount;

		return nextID;
	}

}
