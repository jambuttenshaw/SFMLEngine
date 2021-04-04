#include "CircleCollider.h"

#include "BoxCollider.h"
#include "TilemapCollider.h"

#include "SFMLEngine/Math.h"

namespace SFMLEngine {

	std::pair<bool, sf::FloatRect> CircleCollider::Colliding(CircleCollider& other)
	{
		sf::FloatRect globalBounds = GetGlobalBounds();
		sf::FloatRect otherGlobalBounds = other.GetGlobalBounds();

		// a quick intial check to make sure a rectangle containing the circle overlaps with a rectangle containing the other circle
		if (globalBounds.intersects(otherGlobalBounds))
		{
			// then if the distance between the 2 circles is less than the sum of the square of the radii, there is a collision
			float r = globalBounds.width * 0.5f;
			sf::Vector2f centre{ globalBounds.left + r, globalBounds.top + r };

			float r2 = otherGlobalBounds.width * 0.5f;
			sf::Vector2f centre2{ otherGlobalBounds.left + r, otherGlobalBounds.top + r };

			sf::Vector2f delta{ centre2 - centre };
			if (Math::SquareMagnitude(delta) < (r * r + r2 * r2))
			{
				// return a bounds that best describes the point of collision
				// returning the rectangle that encompasses the circle is not ideal, as the corners of the rectangle are not on the circle
				// using this method the corner of the bounds with be exactly on the collision point
				sf::Vector2f halfExtents{ Math::Normalize(delta) * r };
				return std::make_pair(true, sf::FloatRect{ centre + halfExtents, 2.0f * Math::Abs(halfExtents) });
			}
		}

		return std::make_pair(false, sf::FloatRect{});
	}

	std::pair<bool, sf::FloatRect> CircleCollider::Colliding(BoxCollider& other)
	{
		// use a different overload
		return Colliding(other.GetGlobalBounds());
	}

	std::pair<bool, sf::FloatRect> CircleCollider::Colliding(const sf::Vector2f& point)
	{
		// use a different overload
		return Colliding(point, 0);
	}

	std::pair<bool, sf::FloatRect> CircleCollider::Colliding(const sf::Vector2f& centre, float radius)
	{
		// colliding with a circle defined as a centre point and a radius
		// very similar algorithm to CircleCollider vs CircleCollider collision

		sf::FloatRect globalBounds = GetGlobalBounds();
		float r = globalBounds.width * 0.5f;
		sf::Vector2f d{ globalBounds.left + r - centre.x, globalBounds.top + r - centre.y };

		return std::make_pair(Math::SquareMagnitude(d) < r * r + radius * radius, globalBounds);
	}

	std::pair<bool, sf::FloatRect> CircleCollider::Colliding(const sf::FloatRect& rect)
	{
		// check if the circle is colliding with the rect
		// this is done by clamping the centre of the circle to the rectangle
		// which gives the closest point on the rectangle
		// and then if the distance from this point to the centre is less than the radius there is a collision

		sf::Vector2f otherTopLeft{ rect.left, rect.top };
		sf::Vector2f otherSize{ rect.width, rect.height };

		sf::FloatRect globalBounds = GetGlobalBounds();
		float r = 0.5f * globalBounds.width;
		sf::Vector2f centre{ globalBounds.left + r, globalBounds.top + r };

		sf::Vector2f delta{ centre - Math::Clamp(centre, otherTopLeft, otherTopLeft + otherSize) };

		if (Math::SquareMagnitude(delta) < r * r)
		{
			// collision occurred
			// get a fitting bounds for this collision
			sf::Vector2f halfExtents{ Math::Normalize(delta) * r };
			return std::make_pair(true, sf::FloatRect{ centre + halfExtents, 2.0f * Math::Abs(halfExtents) });
		}
		else
		{
			return std::make_pair(false, sf::FloatRect{});
		}
	}
}
