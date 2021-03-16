#include "BoxCollider.h"

#include "CircleCollider.h"
#include "TilemapCollider.h"

#include "SFMLEngine/Core.h"
#include "SFMLEngine/Math.h"
#include "SFMLEngine/DebugTools.h"


namespace SFMLEngine {

	std::pair<bool, sf::FloatRect> BoxCollider::Colliding(BoxCollider& other)
	{
		ZoneScoped;
		
		if (GetGlobalBounds().intersects(other.GetGlobalBounds()))
		{
			return std::make_pair(true, GetGlobalBounds());
		}
		else
			return std::make_pair(false, sf::FloatRect{});
	}

	std::pair<bool, sf::FloatRect> BoxCollider::Colliding(CircleCollider& other)
	{
		return std::make_pair(false, GetGlobalBounds());
	}

	void BoxCollider::DrawDebug(const sf::Transform& transform)
	{
		auto& transformed = transform.transformRect(Bounds);
		DEBUG_DRAW_RECT(sf::Vector2f(transformed.left, transformed.top),
			sf::Vector2f(transformed.width, transformed.height),
			IsTrigger ? DebugTools::TRIGGER_COLOR : DebugTools::COLLIDER_COLOR);
	}

}
