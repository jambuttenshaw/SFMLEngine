#pragma once


#include "SFMLEngine/ECS/Coordinator.h"


namespace SFMLEngine {

	
	class IdentitySystem : public System
	{
	public:

		void Init(Coordinator* coordinator);

		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override;

		const std::vector<Entity>& GetEntitiesWithTag(const std::string& tag);

		void Update();

	private:

		void UpdateTag(Entity entity);

	private:
		Coordinator* m_Coordinator;

		std::unordered_map<std::string, std::vector<Entity>> m_TagMap;
	};

}
