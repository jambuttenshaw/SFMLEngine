#include "BoxColliderDebugSystem.h"


#include "SFMLEngine/ECS/Components/Transform.h"
#include "SFMLEngine/ECS/Components/Colliders/BoxCollider.h"


namespace SFMLEngine {

	void BoxColliderDebugSystem::Init(Coordinator* coordinator)
	{
		m_Coordinator = coordinator;
	}

	void BoxColliderDebugSystem::EntityAddedToSystem(Entity entity)
	{
		Transform* transform = &m_Coordinator->GetComponent<Transform>(entity);
		BoxCollider* collider = &m_Coordinator->GetComponent<BoxCollider>(entity);

		m_TransformCache.insert(std::make_pair(entity, transform));
		m_ColliderCache.insert(std::make_pair(entity, collider));
	}

	void BoxColliderDebugSystem::EntityRemovedFromSystem(Entity entity)
	{
		m_TransformCache.erase(entity);
		m_ColliderCache.erase(entity);
	}

	void BoxColliderDebugSystem::Update()
	{
		for (auto& entity : m_Entities)
		{
			m_ColliderCache[entity]->DrawDebug(m_TransformCache[entity]->GetTransformMatrix());
		}
	}

}
