#include "Physics.h"


namespace SFMLEngine {

	std::shared_ptr<CollisionSystem> Physics::s_CollisionSystem = nullptr;

	void Physics::Init(std::shared_ptr<CollisionSystem> collisionSystem)
	{
		s_CollisionSystem = collisionSystem;
	}

	/*
	Collision Physics::CollisionAtPoint(const sf::Vector2f& point, float radius, Layer layerMask)
	{
		// run a collision test at the point
		// simulates a circle collider at that point in space
		// and see if it collides with any other colliders
	}*/

}
