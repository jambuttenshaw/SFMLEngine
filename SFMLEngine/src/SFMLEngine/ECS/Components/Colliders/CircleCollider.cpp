#include "CircleCollider.h"

#include "BoxCollider.h"
#include "TilemapCollider.h"

#include "SFMLEngine/Math.h"

namespace SFMLEngine {

	std::pair<bool, sf::FloatRect> CircleCollider::Colliding(CircleCollider& other)
	{
		sf::FloatRect globalBounds = GetGlobalBounds();
		sf::FloatRect otherGlobalBounds = other.GetGlobalBounds();

		if (globalBounds.intersects(otherGlobalBounds))
		{
			float r = globalBounds.width * 0.5f;
			sf::Vector2f centre{ globalBounds.left + r, globalBounds.top + r };

			float r2 = otherGlobalBounds.width * 0.5f;
			sf::Vector2f centre2{ otherGlobalBounds.left + r, otherGlobalBounds.top + r };

			sf::Vector2f delta{ centre2 - centre };
			if (Math::SquareMagnitude(delta) < (r * r + r2 * r2))
			{
				sf::Vector2f halfExtents{ Math::Normalize(delta) * r };
				return std::make_pair(true, sf::FloatRect{ centre + halfExtents, 2.0f * Math::Abs(halfExtents) });
			}
		}

		return std::make_pair(false, sf::FloatRect{});
	}

	std::pair<bool, sf::FloatRect> CircleCollider::Colliding(BoxCollider& other)
	{
		return Colliding(other.GetGlobalBounds());
	}

	std::pair<bool, sf::FloatRect> CircleCollider::Colliding(const sf::Vector2f& point)
	{
		return Colliding(point, 0);
	}

	std::pair<bool, sf::FloatRect> CircleCollider::Colliding(const sf::Vector2f& centre, float radius)
	{
		sf::FloatRect globalBounds = GetGlobalBounds();
		float r = globalBounds.width * 0.5f;
		sf::Vector2f d{ globalBounds.left + r - centre.x, globalBounds.top + r - centre.y };

		return std::make_pair(Math::SquareMagnitude(d) < r * r + radius * radius, globalBounds);
	}

	std::pair<bool, sf::FloatRect> CircleCollider::Colliding(const sf::FloatRect& rect)
	{
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
