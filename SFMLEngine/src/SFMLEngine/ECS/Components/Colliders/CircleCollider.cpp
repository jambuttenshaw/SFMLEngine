#include "CircleCollider.h"

#include "BoxCollider.h"
#include "TilemapCollider.h"

namespace SFMLEngine {

	CollisionData CircleCollider::Colliding(CircleCollider& other, const sf::Vector2f& otherPos)
	{
		return CollisionData{ false, sf::FloatRect() };
	}

	CollisionData CircleCollider::Colliding(BoxCollider& other, const sf::Vector2f& otherPos)
	{
		return CollisionData{ false, sf::FloatRect() };
	}

	CollisionData CircleCollider::Colliding(TilemapCollider& other, const sf::Vector2f& otherPos)
	{
		return CollisionData{ false, sf::FloatRect() };
	}

}
