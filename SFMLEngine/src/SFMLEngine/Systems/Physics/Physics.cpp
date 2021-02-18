#include "Physics.h"


namespace SFMLEngine {

	std::shared_ptr<CollisionSystem> Physics::s_CollisionSystem = nullptr;

	void Physics::Init(std::shared_ptr<CollisionSystem> collisionSystem)
	{
		s_CollisionSystem = collisionSystem;
	}

}
