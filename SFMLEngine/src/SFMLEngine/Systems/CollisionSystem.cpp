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

	const Collision CollisionSystem::TestCollision(const ICollider& collider)
	{
		switch (collider.Type)
		{
		case ColliderType::Invalid:	SFMLE_CORE_ASSERT(0, "Invalid collider type!"); break;
		case ColliderType::Box:		return DoCollisionTest(dynamic_cast<const BoxCollider&>(collider)); break;
		case ColliderType::Circle:	return DoCollisionTest(dynamic_cast<const CircleCollider&>(collider)); break;
		default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
		}

		return Collision{ false, NULL_ENTITY_ID };
	}

}
