#pragma once

#include "../Constants.h"
#include "../Log.h"

#include <cassert>
#include <queue>
#include <unordered_map>


namespace SFMLEngine {

	class ResourceManager
	{
	public:
		static void Init();

		template<typename T>
		static ResourceID LoadFromFile(std::string filepath)
		{
			// create a new instance of the resource
			T* resource = new T;
			
			// load if from the file
			if (!resource->loadFromFile(filepath))
			{
				// error- resource could not be loaded from file
				LOG_ERROR("Resource could not be loaded from {0}", filepath);
				delete resource;
				return NULL_RESOURCE_ID;
			}

			// create a new ID for the resource
			ResourceID newID = GetNextID();

			// add the pointer to the resource into the resource manager
			s_Resources.insert(std::make_pair(newID, static_cast<ResourceHandle>(resource)));

			// return the ID used to access the resource
			return newID;
		}

		template<typename T>
		static ResourceID LoadFromFile(const std::string& filepath1, const std::string& filepath2)
		{
			// create a new instance of the resource
			T* resource = new T;

			// load if from the file
			if (!resource->loadFromFile(filepath1, filepath2))
			{
				// error- resource could not be loaded from file
				LOG_ERROR("Resource could not be loaded from files: {0} {1}", filepath1, filepath2);
				delete resource;
				return NULL_RESOURCE_ID;
			}

			// create a new ID for the resource
			ResourceID newID = GetNextID();

			// add the pointer to the resource into the resource manager
			s_Resources.insert(std::make_pair(newID, static_cast<ResourceHandle>(resource)));

			// return the ID used to access the resource
			return newID;
		}

		template<typename T>
		static void DeleteResource(ResourceID resourceID)
		{
			auto location = s_Resources.find(resourceID);
			// make sure that we actually found a resource associated with that ID
			assert(location != s_Resources.end() && "Invalid ResourceID: no resource could be found!");

			delete static_cast<T*>(s_Resources.at(location));
			s_Resources.erase(location);
		}

		template<typename T>
		static T* GetResourceHandle(ResourceID resourceID)
		{
			// find the resource in the resources map
			auto location = s_Resources.find(resourceID);

			// make sure that we actually found a resource associated with that ID
			assert(location != s_Resources.end() && "Invalid ResourceID: no resource could be found!");

			// cast the resource then return it
			// resources are stored in the map as ResourceHandle's, which are just void*
			return static_cast<T*>(s_Resources.at(resourceID));
		}

	private:
		static ResourceID GetNextID();

	private:
		static std::queue<ResourceID> s_AvailableResourceIDs;
		static size_t s_LivingResourceCount;

		static std::unordered_map<ResourceID, ResourceHandle> s_Resources;
	};
}
