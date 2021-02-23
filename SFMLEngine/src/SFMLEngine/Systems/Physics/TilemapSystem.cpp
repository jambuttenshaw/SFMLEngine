#include "TilemapSystem.h"


#include "SFMLEngine/ECS/Components/Transform.h"
#include "SFMLEngine/ECS/Components/Tilemap.h"
#include "SFMLEngine/ECS/Components/Colliders/TilemapCollider.h"


namespace SFMLEngine {

	void TilemapSystem::Init(Coordinator* coordinator)
	{
		m_Coordinator = coordinator;
	}

	void TilemapSystem::EntityAddedToSystem(Entity entity)
	{
		Transform* transform = &m_Coordinator->GetComponent<Transform>(entity);
		Tilemap* tilemap = &m_Coordinator->GetComponent<Tilemap>(entity);
		TilemapCollider* collider = &m_Coordinator->GetComponent<TilemapCollider>(entity);

		collider->Setup(tilemap);

		m_TransformCache.insert(std::make_pair(entity, transform));
		m_TilemapCache.insert(std::make_pair(entity, tilemap));
		m_ColliderCache.insert(std::make_pair(entity, collider));
	}
	
	void TilemapSystem::EntityRemovedFromSystem(Entity entity)
	{
		m_TransformCache.erase(entity);
		m_TilemapCache.erase(entity);
		m_ColliderCache.erase(entity);
	}

	void TilemapSystem::Update()
	{
		ZoneScoped;

		for (auto& entity : m_Entities)
		{
			m_ColliderCache[entity]->DrawDebug(m_TransformCache[entity]->GetWorldTransformMatrix());
			if (ComponentModified(m_TilemapCache[entity]))
			{
				m_ColliderCache[entity]->UpdateGeometry();

				ResetModified(m_TilemapCache[entity]);
			}
		}
	}

}
