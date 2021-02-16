#include "CircleCollider.h"

#include "BoxCollider.h"
#include "TilemapCollider.h"

#include "SFMLEngine/Math.h"

namespace SFMLEngine {

	CollisionData CircleCollider::Colliding(CircleCollider& other, const sf::Vector2f& otherPos)
	{
		// circle vs circle collision
		// this is simple
		// check if the distance between the centres is less than the sum of the radii
		sf::Vector2f thisCentre{ Offset.x + Radius, Offset.y + Radius };
		sf::Vector2f otherCentre{ other.Offset.x + other.Radius + otherPos.x, other.Offset.y + other.Radius + otherPos.y };

		sf::Vector2f d = otherCentre - thisCentre;

		bool collision = Math::Dot(d, d) <= Radius * Radius + other.Radius * other.Radius;

		return CollisionData{ collision, sf::FloatRect{ Offset, sf::Vector2f(Radius, Radius) } };
	}

	CollisionData CircleCollider::Colliding(BoxCollider& other, const sf::Vector2f& otherPos)
	{
		return other.Colliding(*this, otherPos);
	}

	CollisionData CircleCollider::Colliding(TilemapCollider& other, const sf::Vector2f& otherPos)
	{
		return CollisionData{ false, sf::FloatRect() };
	}

}
