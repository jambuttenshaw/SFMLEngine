#include "Collider.h"

#include "SFMLEngine/Systems/Physics/CollisionSystem.h"


namespace SFMLEngine {

	// any set up that needs to be applied to all colliders,
	// regardless of shape
	void Collider::Init()
	{
		m_ColliderID = CollisionSystem::GetNextColliderID();
	}

}
