#include "CircleCollider.h"

#include "BoxCollider.h"

namespace SFMLEngine {

	CollisionData CircleCollider::Colliding(const CircleCollider& other, const sf::Vector2f& otherPos)
	{
		return CollisionData{ false, sf::Vector2f() };
	}

	CollisionData CircleCollider::Colliding(const BoxCollider& other, const sf::Vector2f& otherPos)
	{
		return CollisionData{ false, sf::Vector2f() };
	}

}
