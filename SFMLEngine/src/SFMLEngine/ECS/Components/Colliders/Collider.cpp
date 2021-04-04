#include "Collider.h"

#include "SFMLEngine/Systems/Physics/CollisionSystem.h"


namespace SFMLEngine {

	// any set up that needs to be applied to all colliders,
	// regardless of shape
	void Collider::Init()
	{
		// simply retrieves an id from the collision system
		m_ColliderID = CollisionSystem::GetNextColliderID();
	}

	void Collider::SetNewColliderID()
	{
		// gets a new collider id

		// delete the old collider ID if it isnt null
		if (m_ColliderID != NULL_COLLIDER_ID)
			CollisionSystem::DeletedColliderID(m_ColliderID);

		m_ColliderID = CollisionSystem::GetNextColliderID();
	}

}
