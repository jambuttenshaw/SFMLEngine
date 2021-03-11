#include "CollisionSystem.h"

#include "SFMLEngine/Systems/Physics/Physics.h"

#include "SFMLEngine/ECS/Components/Transform.h"
#include "SFMLEngine/ECS/Components/Colliders/BoxCollider.h"
#include "SFMLEngine/ECS/Components/Colliders/CircleCollider.h"
#include "SFMLEngine/ECS/Components/Colliders/TilemapCollider.h"


namespace SFMLEngine {

	std::queue<ColliderID> CollisionSystem::s_AvailableColliderIDs;
	size_t CollisionSystem::s_LivingColliderCount = 0;


	void CollisionSystem::Init(Coordinator* coordinator)
	{
		m_Coordinator = coordinator;
	}

	void CollisionSystem::EntityAddedToSystem(Entity entity)
	{
		ZoneScoped;

		Transform* transform = &m_Coordinator->GetComponent<Transform>(entity);
		auto const& collider = m_Coordinator->GetComponent<ColliderInfo>(entity);

		switch (collider.Type)
		{
		case ColliderType::Invalid:	SFMLE_CORE_ASSERT(0, "Invalid collider type!"); break;
		case ColliderType::Box:		m_Coordinator->GetComponent<BoxCollider>(entity).SetTransform(transform); break;
		case ColliderType::Circle:	m_Coordinator->GetComponent<CircleCollider>(entity).SetTransform(transform); break;
		case ColliderType::Tilemap: m_Coordinator->GetComponent<TilemapCollider>(entity).SetTransform(transform); break;
		default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
		}
	}

	void CollisionSystem::EntityRemovedFromSystem(Entity entity)
	{

	}

	const std::vector<Collision> CollisionSystem::TestCollision(Entity entity)
	{
		ZoneScoped;

		auto const& collider = m_Coordinator->GetComponent<ColliderInfo>(entity);
		auto mask = Physics::GetLayerMask(m_Coordinator->GetComponent<Identity>(entity).EntityLayer);

		switch (collider.Type)
		{
		case ColliderType::Invalid:	SFMLE_CORE_ASSERT(0, "Invalid collider type!"); break;
		case ColliderType::Box:		return DoCollisionTest(m_Coordinator->GetComponent<BoxCollider>(entity), mask, entity); break;
		case ColliderType::Circle:	return DoCollisionTest(m_Coordinator->GetComponent<CircleCollider>(entity), mask, entity); break;
		case ColliderType::Tilemap: return DoCollisionTest(m_Coordinator->GetComponent<TilemapCollider>(entity), mask, entity); break;
		default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
		}

		// this shouldn't be reached
		return std::vector<Collision>();
	}






	void CollisionSystem::SetupColliderIDs()
	{
		for (ColliderID colliderID = 0; colliderID < MAX_COLLIDERS; ++colliderID)
		{
			s_AvailableColliderIDs.push(colliderID);
		}
	}

	ColliderID CollisionSystem::GetNextColliderID()
	{
		SFMLE_CORE_ASSERT(s_LivingColliderCount < MAX_COLLIDERS, "Too many colliders in existance!");

		ColliderID nextID = s_AvailableColliderIDs.front();
		s_AvailableColliderIDs.pop();
		++s_LivingColliderCount;

		return nextID;
	}

	void CollisionSystem::ReplaceColliderID(ColliderID id)
	{
		if (id == NULL_COLLIDER_ID) return;

		s_AvailableColliderIDs.push(id);
		--s_LivingColliderCount;
	}


}
