#include "CollisionSystem.h"

#include "../ECS/Components/Transform.h"
#include "../ECS/Components/Colliders/BoxCollider.h"
#include "../ECS/Components/Colliders/CircleCollider.h"


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
		auto const& collider = m_Coordinator->GetComponent<Collider>(entity);
		switch (collider.Type)
		{
		case ColliderType::Box:		return DoCollisionTest(m_Coordinator->GetComponent<BoxCollider>(entity)); break;
		case ColliderType::Circle:	return DoCollisionTest(m_Coordinator->GetComponent<CircleCollider>(entity)); break;
		default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
		}

		return Collision{ false, NULL_ENTITY_ID };
	}

}
