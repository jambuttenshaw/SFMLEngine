#include "BoxCollider.h"

#include "CircleCollider.h"
#include "TilemapCollider.h"

#include "SFMLEngine/Core.h"
#include "SFMLEngine/Math.h"
#include "SFMLEngine/DebugTools.h"


namespace SFMLEngine {

	std::pair<bool, sf::FloatRect> BoxCollider::Colliding(BoxCollider& other)
	{
		return std::make_pair(GetGlobalBounds().intersects(other.GetGlobalBounds()), GetGlobalBounds());
	}

	std::pair<bool, sf::FloatRect> BoxCollider::Colliding(CircleCollider& other)
	{
		return std::make_pair(other.Colliding(*this).first, GetGlobalBounds());
	}

	std::pair<bool, sf::FloatRect> BoxCollider::Colliding(const sf::Vector2f& point)
	{
		return std::make_pair(GetGlobalBounds().contains(point), GetGlobalBounds());
	}

	std::pair<bool, sf::FloatRect> BoxCollider::Colliding(const sf::Vector2f& centre, float radius)
	{
		auto [topLeft, bottomRight] = Math::GetCorners(GetGlobalBounds());

		// get closest point on box to centre of circle
		sf::Vector2f closestPoint{ Math::Clamp(centre, topLeft, bottomRight) };
		sf::Vector2f d = centre - closestPoint;

		if (Math::SquareMagnitude(d) < radius * radius)
		{
			return std::make_pair(true, GetGlobalBounds());
		}
		else
			return std::make_pair(false, sf::FloatRect{});
	}

	std::pair<bool, sf::FloatRect> BoxCollider::Colliding(const sf::FloatRect& other)
	{
		return std::make_pair(GetGlobalBounds().intersects(other), GetGlobalBounds());
	}

	void BoxCollider::DrawDebug(const sf::Transform& transform)
	{
		auto& transformed = transform.transformRect(Bounds);
		DEBUG_DRAW_RECT(sf::Vector2f(transformed.left, transformed.top),
			sf::Vector2f(transformed.width, transformed.height),
			IsTrigger ? DebugTools::TRIGGER_COLOR : DebugTools::COLLIDER_COLOR);
	}
}
