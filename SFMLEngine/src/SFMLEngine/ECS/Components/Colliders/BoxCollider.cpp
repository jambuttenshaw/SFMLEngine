#include "BoxCollider.h"

#include "CircleCollider.h"
#include "TilemapCollider.h"

#include "SFMLEngine/Log.h"
#include "SFMLEngine/Math.h"
#include "SFMLEngine/DebugTools.h"


namespace SFMLEngine {

	std::pair<bool, sf::FloatRect> BoxCollider::Colliding(BoxCollider& other)
	{
		ZoneScoped;

		sf::FloatRect& thisWorldPos = GetGlobalBounds();
		sf::FloatRect& otherWorldPos = other.GetGlobalBounds();

		bool overlapX = (thisWorldPos.left + thisWorldPos.width >= otherWorldPos.left) && (otherWorldPos.left + otherWorldPos.width >= thisWorldPos.left);
		bool overlapY = (thisWorldPos.top + thisWorldPos.height >= otherWorldPos.top) && (otherWorldPos.top + otherWorldPos.height >= thisWorldPos.top);

		return std::make_pair(overlapX && overlapY, thisWorldPos);
	}

	std::pair<bool, sf::FloatRect> BoxCollider::Colliding(CircleCollider& other)
	{
		/*
		sf::Vector2f circleCentre = other.Offset + otherPos + sf::Vector2f{other.Radius, other.Radius};

		sf::Vector2f half_extents{ Size * 0.5f };
		sf::Vector2f centre{ Offset + half_extents };

		sf::Vector2f diff = centre - circleCentre + Math::Clamp(circleCentre - centre, -half_extents, half_extents);

		return CollisionData{ Math::SquareMagnitude(diff) <= other.Radius * other.Radius, sf::FloatRect{ Offset, Size }, Offset };
		*/
		return std::make_pair(false, GetGlobalBounds());
	}

	std::pair<bool, sf::FloatRect> BoxCollider::Colliding(TilemapCollider& other)
	{
		/*
		// tilemap vs box collider collision
		ZoneScoped;
		auto& collision = other.Colliding(*this, otherPos);
		return CollisionData{ collision.Collided, sf::FloatRect{ Offset, Size }, Offset };
		*/
			
		return std::make_pair(false, GetGlobalBounds());
	}

	void BoxCollider::DrawDebug(const sf::Transform& transform)
	{
		auto& transformed = transform.transformRect(sf::FloatRect(Offset.x, Offset.y, Size.x, Size.y));
		DEBUG_DRAW_RECT(sf::Vector2f(transformed.left, transformed.top), sf::Vector2f(transformed.width, transformed.height), sf::Color::Green);
	}

}
