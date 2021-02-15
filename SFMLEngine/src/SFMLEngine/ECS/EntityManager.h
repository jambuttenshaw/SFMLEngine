#pragma once

#include "SFMLEngine/Constants.h"

#include <array>
#include <queue>

namespace SFMLEngine {

	class EntityManager
	{
	public:
		EntityManager();
		~EntityManager() = default;

		Entity CreateEntity();
		void DestroyEntity(Entity entity);

		bool HasComponentInSignature(Entity entity, ComponentType component);

		void SetSignature(Entity entity, Signature signature);
		Signature GetSignature(Entity entity);

	private:
		// Queue of unused entity IDs
		std::queue<Entity> m_AvailableEntities{};

		// Array of signatures where the index corresponds to the entity ID
		std::array<Signature, MAX_ENTITIES> m_Signatures{};

		// Total living entities - used to keep limits on how many exist
		uint32_t m_LivingEntityCount{};
	};

}
