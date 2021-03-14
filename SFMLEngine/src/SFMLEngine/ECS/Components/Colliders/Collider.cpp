#include "Collider.h"

#include "SFMLEngine/Systems/Physics/CollisionSystem.h"


namespace SFMLEngine {

	// any set up that needs to be applied to all colliders,
	// regardless of shape
	void Collider::Init()
	{
		m_ColliderID = CollisionSystem::GetNextColliderID();
	}

	void Collider::SetNewColliderID()
	{
		// delete the old collider ID if it isnt null
		if (m_ColliderID != NULL_COLLIDER_ID)
			CollisionSystem::ReplaceColliderID(m_ColliderID);

		m_ColliderID = CollisionSystem::GetNextColliderID();
	}

}
