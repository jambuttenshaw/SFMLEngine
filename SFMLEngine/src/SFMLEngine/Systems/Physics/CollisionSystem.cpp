#include "CollisionSystem.h"

#include "SFMLEngine/Systems/Physics/Physics.h"

#include "SFMLEngine/ECS/Components/Transform.h"
#include "SFMLEngine/ECS/Components/Colliders/BoxCollider.h"
#include "SFMLEngine/ECS/Components/Colliders/CircleCollider.h"
#include "SFMLEngine/ECS/Components/Colliders/TilemapCollider.h"


namespace SFMLEngine {

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

	const Collision CollisionSystem::TestCollision(Entity entity)
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

		return Collision{ false, INVALID_ENTITY_ID };
	}
}
