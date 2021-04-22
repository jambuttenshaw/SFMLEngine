#include "DataStore.h"


namespace SFMLEngine {

	std::unordered_map<std::string, void*> DataStore::s_StoredData;

	void DataStore::Shutdown()
	{
		for (auto& pair : s_StoredData)
		{
			LOG_CORE_WARN("Data '{0}' still existing in data store at shutdown: Persistent data should be deleted manually.");
			delete pair.second;
		}
		s_StoredData.clear();
	}

}