#pragma once

#include "CollisionSystem.h"


namespace SFMLEngine {

	class Physics
	{
	public:
		static void Init(std::shared_ptr<CollisionSystem> collisionSystem);

		static Collision CollisionAtPoint(const sf::Vector2f& point, float radius, Layer layerMask = Layer{});

	public:
		static inline const sf::Vector2f Gravity = {0, 750};

	private:
		static std::shared_ptr<CollisionSystem> s_CollisionSystem;
	};

}
