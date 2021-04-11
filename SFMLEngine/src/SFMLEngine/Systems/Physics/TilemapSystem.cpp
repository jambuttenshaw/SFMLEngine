#include "TilemapSystem.h"


#include "SFMLEngine/ECS/Components/Transform.h"
#include "SFMLEngine/ECS/Components/Tilemap.h"
#include "SFMLEngine/ECS/Components/Colliders/TilemapCollider.h"

#include "SFMLEngine/Systems/Physics/CollisionSystem.h"


namespace SFMLEngine {

	void TilemapSystem::Init(Coordinator* coordinator, std::shared_ptr<CollisionSystem> collisionSystem)
	{
		m_Coordinator = coordinator;
		m_CollisionSystem = collisionSystem;
	}

	void TilemapSystem::EntityAddedToSystem(Entity entity)
	{
		Transform* transform = &m_Coordinator->GetComponent<Transform>(entity);
		
		Tilemap* tilemap = &m_Coordinator->GetComponent<Tilemap>(entity);
		tilemap->SetTransform(transform);

		TilemapCollider* collider = &m_Coordinator->GetComponent<TilemapCollider>(entity);

		collider->Setup(tilemap);

		m_TransformCache.insert(std::make_pair(entity, transform));
		m_TilemapCache.insert(std::make_pair(entity, tilemap));
		m_ColliderCache.insert(std::make_pair(entity, collider));
	}
	
	void TilemapSystem::EntityRemovedFromSystem(Entity entity)
	{
		m_TilemapCache[entity]->SetTransform(nullptr);

		m_TransformCache.erase(entity);
		m_TilemapCache.erase(entity);
		m_ColliderCache.erase(entity);
	}

	void TilemapSystem::Update()
	{
		ZoneScoped;

		for (auto& entity : m_Entities)
		{
			if (m_ColliderCache[entity]->DisplayDebug())
				m_ColliderCache[entity]->DrawDebug(m_TransformCache[entity]->GetLocalToWorldTransformMatrix());


			if (ComponentModified(m_TilemapCache[entity]))
			{
				m_CollisionSystem->DeleteTilemapColliderData(entity);
				m_ColliderCache[entity]->UpdateGeometry();
				m_CollisionSystem->AddTilemapColliderData(entity);

				ResetModified(m_TilemapCache[entity]);
			}
		}
	}

}
