#include "BoxCollider.h"

#include "CircleCollider.h"
#include "TilemapCollider.h"

#include "SFMLEngine/Core.h"
#include "SFMLEngine/Math.h"
#include "SFMLEngine/DebugTools.h"


namespace SFMLEngine {

	std::pair<bool, sf::FloatRect> BoxCollider::Colliding(BoxCollider& other)
	{
		// ncie and quick, SFML gives us a function for checking overlap of rectangles
		return std::make_pair(GetGlobalBounds().intersects(other.GetGlobalBounds()), GetGlobalBounds());
	}

	std::pair<bool, sf::FloatRect> BoxCollider::Colliding(CircleCollider& other)
	{
		// use the circle colliders function for checking collision
		return std::make_pair(other.Colliding(*this).first, GetGlobalBounds());
	}

	std::pair<bool, sf::FloatRect> BoxCollider::Colliding(const sf::Vector2f& point)
	{
		return std::make_pair(GetGlobalBounds().contains(point), GetGlobalBounds());
	}

	std::pair<bool, sf::FloatRect> BoxCollider::Colliding(const sf::Vector2f& centre, float radius)
	{
		// collision between a box collider and a circle defined as a centre and a radius
		// same algorithm as in CircleCollider::Colliding with a rectangle
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
		// draw a box showing the shape of the collider
		// very useful for debugging
		auto& transformed = transform.transformRect(m_Bounds);
		DEBUG_DRAW_RECT(sf::Vector2f(transformed.left, transformed.top),
			sf::Vector2f(transformed.width, transformed.height),
			IsTrigger() ? DebugTools::TRIGGER_COLOR : DebugTools::COLLIDER_COLOR);
	}
}
