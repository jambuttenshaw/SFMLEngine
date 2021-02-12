#include "TilemapSystem.h"


#include "../ECS/Components/Tilemap.h"
#include "../ECS/Components/Colliders/TilemapCollider.h"


namespace SFMLEngine {

	void TilemapSystem::Init(Coordinator* coordinator)
	{
		m_Coordinator = coordinator;
	}

	void TilemapSystem::EntityAddedToSystem(Entity entity)
	{
		Tilemap* tilemap = &m_Coordinator->GetComponent<Tilemap>(entity);
		TilemapCollider* collider = &m_Coordinator->GetComponent<TilemapCollider>(entity);

		collider->Setup(tilemap);

		m_TilemapCache.insert(std::make_pair(entity, tilemap));
		m_ColliderCache.insert(std::make_pair(entity, collider));
	}
	
	void TilemapSystem::EntityRemovedFromSystem(Entity entity)
	{
		m_TilemapCache.erase(entity);
		m_ColliderCache.erase(entity);
	}

	void TilemapSystem::Update()
	{
		for (auto& entity : m_Entities)
		{
			m_ColliderCache[entity]->DrawDebug();
			if (ComponentModified(m_TilemapCache[entity]))
			{
				m_ColliderCache[entity]->UpdateGeometry();

				ResetModified(m_TilemapCache[entity]);
			}
		}
	}

}
