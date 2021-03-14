#include "CircleCollider.h"

#include "BoxCollider.h"
#include "TilemapCollider.h"

#include "SFMLEngine/Math.h"

namespace SFMLEngine {

	std::pair<bool, sf::FloatRect> CircleCollider::Colliding(CircleCollider& other)
	{
		// circle vs circle collision
		auto& thisGlobalBounds = GetGlobalBounds();
		sf::Vector2f thisCentre{ thisGlobalBounds.left + Radius, thisGlobalBounds.top + Radius };

		auto& otherGlobalBounds = other.GetGlobalBounds();
		sf::Vector2f otherCentre{ otherGlobalBounds.left + other.Radius, otherGlobalBounds.top + other.Radius };

		sf::Vector2f d = otherCentre - thisCentre;
		// they collide if the distance between the centres is less than the sum of their radii
		bool collision = Math::SquareMagnitude(d) <= Radius * Radius + other.Radius * other.Radius;

		if (collision)
		{
			// compute a bounding box that is particular to the point on the edge of the circle
			// that the collision occurred
			// since an AABB wont accurately represent the shape of the circle
			sf::Vector2f toEdge = Radius * Math::Normalize(d);
			sf::Vector2f halfExtents{ fabsf(Math::Dot(toEdge, {1, 0})), fabsf(Math::Dot(toEdge, {0, 1})) };

			return std::make_pair(true, sf::FloatRect(thisCentre - halfExtents, 2.0f * halfExtents));
		}
		else
			return std::make_pair(false, sf::FloatRect{});
	}

	std::pair<bool, sf::FloatRect> CircleCollider::Colliding(BoxCollider& other)
	{
		return std::make_pair(other.Colliding(*this).first, GetGlobalBounds());
	}

}
