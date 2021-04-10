#include "IdentitySystem.h"

#include "SFMLEngine/ECS/Components/Identity.h"


namespace SFMLEngine {

	void IdentitySystem::Init(Coordinator* coordinator)
	{
		m_Coordinator = coordinator;
	}

	void IdentitySystem::EntityAddedToSystem(Entity entity)
	{
		UpdateTag(entity);
	}

	void IdentitySystem::EntityRemovedFromSystem(Entity entity)
	{
		auto& id = m_Coordinator->GetComponent<Identity>(entity);

		auto& entities = m_TagMap[id.GetTag()];

		size_t index = 0;
		for (auto const& e : entities)
		{
			if (e == entity)
				break;
			index++;
		}

		if (index < entities.size()) entities.erase(entities.begin() + index);
	}

	const std::vector<Entity>& IdentitySystem::GetEntitiesWithTag(const std::string& tag)
	{
		return m_TagMap[tag];
	}


	void IdentitySystem::Update()
	{
		ZoneScoped;

		for (auto const& entity : m_Entities)
		{
			auto& id = m_Coordinator->GetComponent<Identity>(entity);
			if (ComponentModified(id))
			{
				UpdateTag(entity);

				ResetModified(id);
			}
		}
	}

	void IdentitySystem::UpdateTag(Entity entity)
	{
		auto& id = m_Coordinator->GetComponent<Identity>(entity);

		// check that the entity actually has a tag
		if (id.GetTag() == "NULL") return;


		// check if the tag already exists
		if (m_TagMap.find(id.GetTag()) == m_TagMap.end())
		{
			// this is a new tag
			std::vector<Entity> entities{ entity };
			m_TagMap.insert(std::make_pair(id.GetTag(), entities));
		}
		else
		{
			// this is not a new tag
			// add this entity to the vector of entites associated with this tag

			// check this entity hasn't already been registered with this tag
			const std::string& tag = id.GetTag();
			bool found = false;
			for (auto& e : m_TagMap[tag])
			{
				if (e == entity)
				{
					found = true;
					break;
				}
			}
			if (!found) m_TagMap[tag].push_back(entity);
		}
	}

}