#include "CircleCollider.h"

#include "BoxCollider.h"
#include "TilemapCollider.h"

#include "SFMLEngine/Math.h"

namespace SFMLEngine {

	std::pair<bool, sf::FloatRect> CircleCollider::Colliding(CircleCollider& other)
	{
		/*
		// circle vs circle collision
		// this is simple
		// check if the distance between the centres is less than the sum of the radii
		sf::Vector2f thisCentre{ Offset.x + Radius, Offset.y + Radius };
		sf::Vector2f otherCentre{ other.Offset.x + other.Radius + otherPos.x, other.Offset.y + other.Radius + otherPos.y };

		sf::Vector2f d = otherCentre - thisCentre;

		bool collision = Math::Dot(d, d) <= Radius * Radius + other.Radius * other.Radius;

		return CollisionData{ collision, sf::FloatRect{ Offset, sf::Vector2f(Radius, Radius) } };
		*/
		return std::make_pair(false, GetGlobalBounds());
	}

	std::pair<bool, sf::FloatRect> CircleCollider::Colliding(BoxCollider& other)
	{
		/*
		sf::Vector2f semiSize{ Radius, Radius };
		return CollisionData{ other.Colliding(*this, otherPos).Collided, sf::FloatRect{ Offset, 2.0f * semiSize }, 2.0f * semiSize };
		*/
		return std::make_pair(false, GetGlobalBounds());
	}

	std::pair<bool, sf::FloatRect> CircleCollider::Colliding(TilemapCollider& other)
	{
		return std::make_pair(false, GetGlobalBounds());
	}

}
