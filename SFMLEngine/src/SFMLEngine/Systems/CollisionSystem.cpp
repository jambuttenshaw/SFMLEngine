#include "CollisionSystem.h"

#include "../ECS/Components/Transform.h"
#include "../ECS/Components/Colliders/BoxCollider.h"
#include "../ECS/Components/Colliders/CircleCollider.h"
#include "../ECS/Components/Colliders/TilemapCollider.h"


namespace SFMLEngine {

	void CollisionSystem::Init(Coordinator* coordinator)
	{
		m_Coordinator = coordinator;
	}

	void CollisionSystem::EntityAddedToSystem(Entity entity)
	{

	}

	void CollisionSystem::EntityRemovedFromSystem(Entity entity)
	{

	}

	const Collision CollisionSystem::TestCollision(Entity entity)
	{
		ZoneScoped;

		auto const& collider = m_Coordinator->GetComponent<Collider>(entity);
		auto const& transform = m_Coordinator->GetComponent<Transform>(entity);

		switch (collider.Type)
		{
		case ColliderType::Invalid:	SFMLE_CORE_ASSERT(0, "Invalid collider type!"); break;
		case ColliderType::Box:		return DoCollisionTest(m_Coordinator->GetComponent<BoxCollider>(entity), transform.Position, entity); break;
		case ColliderType::Circle:	return DoCollisionTest(m_Coordinator->GetComponent<CircleCollider>(entity), transform.Position, entity); break;
		case ColliderType::Tilemap: return DoCollisionTest(m_Coordinator->GetComponent<TilemapCollider>(entity), transform.Position, entity); break;
		default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
		}

		return Collision{ false, INVALID_ENTITY_ID };
	}

}
