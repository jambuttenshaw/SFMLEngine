#include "Physics.h"


#include "SFMLEngine/ECS/Components/Colliders/CircleCollider.h"
#include "SFMLEngine/ECS/Components/Transform.h"

#include "SFMLEngine/DebugTools.h"

namespace SFMLEngine {

	std::shared_ptr<CollisionSystem> Physics::s_CollisionSystem = nullptr;

	void Physics::Init(std::shared_ptr<CollisionSystem> collisionSystem)
	{
		s_CollisionSystem = collisionSystem;
	}

	
	Collision Physics::CollisionAtPoint(const sf::Vector2f& point, float radius, Layer layerMask)
	{
		// run a collision test at the point
		// creates a circle collider at that point in space
		// and see if it collides with any other colliders

		CircleCollider collider{ radius, point - sf::Vector2f{ radius, radius} };
		Transform t{};
		collider.SetTransform(&t);

		DebugTools::DrawRect(point - sf::Vector2f{ radius, radius }, 2.0f * sf::Vector2f{ radius, radius }, sf::Color::Red);

		return s_CollisionSystem->DoCollisionTest(collider);
	}

}
