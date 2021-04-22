#pragma once

#include "SFMLEngine/Core.h"
#include "SFMLEngine/Constants.h"

#include <unordered_map>

/*

The data store is used to hold data that is required to last between scenes.
Its purpose is to be used by clients of the engine.


While the data store features a lot of similar functionality to the resource manager,
it differs in that data is accessed through a user-defined handle, rather than a engine-generated ID.

This solves the problem of requiring the ID to persist between scenes, as any data in the store should have its handle known at all times.

*/


namespace SFMLEngine {

	class DataStore
	{
	public:

		static void Shutdown();


		template<typename T>
		static void CreatePersistentData(const std::string& dataHandle, T data)
		{
			SFMLE_CORE_ASSERT(s_StoredData.find(dataHandle) == s_StoredData.end(), "Multiple data items cannot share a handle!");

			T* copy = new T(data);
			s_StoredData.insert({ dataHandle, static_cast<void*>(copy) });
		}

		template<typename T>
		static T* RetrieveData(const std::string& dataHandle)
		{
			SFMLE_CORE_ASSERT(s_StoredData.find(dataHandle) != s_StoredData.end(), "No data exists witht that handle!");

			return static_cast<T*>(s_StoredData[dataHandle]);
		}

		template<typename T>
		static void DeleteData(const std::string& dataHandle)
		{
			SFMLE_CORE_ASSERT(s_StoredData.find(dataHandle) != s_StoredData.end(), "No data exists witht that handle!");

			delete static_cast<T*>(s_StoredData[dataHandle]);
			s_StoredData.erase(dataHandle);
		}


	private:
		static std::unordered_map<std::string, void*> s_StoredData;
	};

}
