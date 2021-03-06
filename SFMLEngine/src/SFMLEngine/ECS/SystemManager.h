#pragma once

#include "SFMLEngine/Constants.h"
#include "System.h"

#include <memory>
#include <unordered_map>

namespace SFMLEngine {

	class SystemManager
	{
	public:
		template<typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			const char* typeName = typeid(T).name();

			SFMLE_CORE_ASSERT(m_Systems.find(typeName) == m_Systems.end(), "Registering system more than once.");


			// create a pointer to the system and return it so it can be used externally
			auto system = std::make_shared<T>();
			m_Systems.insert({ typeName, system });
			return system;
		}

		template<typename T>
		void SetSignature(Signature signature)
		{
			const char* typeName = typeid(T).name();

			SFMLE_CORE_ASSERT(m_Systems.find(typeName) != m_Systems.end(), "System used before registered.");

			// Set the signature for this system
			m_Signatures.insert({ typeName, signature });
		}

		void EntityDestroyed(Entity entity)
		{
			// Erase a destroyed entity from all system lists
			// mEntities is a set so no check needed
			for (auto const& pair : m_Systems)
			{
				auto const& system = pair.second;

				if (system->m_Entities.find(entity) != system->m_Entities.end()) system->EntityRemovedFromSystem(entity);
				system->m_Entities.erase(entity);
			}
		}

		void EntitySignatureChanged(Entity entity, Signature entitySignature)
		{
			// Notify each system that an entity's signature changed
			for (auto const& pair : m_Systems)
			{
				auto const& type = pair.first;
				auto const& system = pair.second;
				auto const& systemSignature = m_Signatures[type];

				// Entity signature matches system signature - insert into set
				if ((entitySignature & systemSignature) == systemSignature)
				{
					// make sure the system doesnt already contain the system
					if (system->m_Entities.find(entity) == system->m_Entities.end()) system->EntityAddedToSystem(entity);

					system->m_Entities.insert(entity);
				}
				// Entity signature does not match system signature - erase from set
				else
				{
					if (system->m_Entities.find(entity) != system->m_Entities.end()) system->EntityRemovedFromSystem(entity);

					system->m_Entities.erase(entity);
				}
			}
		}

	private:
		// a map holding the signature of each system
		std::unordered_map<const char*, Signature> m_Signatures{};

		// a map holding the pointer to each system
		std::unordered_map<const char*, std::shared_ptr<System>> m_Systems{};

	};



}
