#pragma once

#include <SFML/Graphics.hpp>

#include "Collider.h"

#include "SFMLEngine/ECS/System.h"


namespace SFMLEngine {

	struct CircleCollider;
	struct TilemapCollider;

	struct BoxCollider : public Collider
	{
		sf::FloatRect Bounds;

		BoxCollider()
			: Bounds()
		{}
		BoxCollider(const sf::Vector2f& size, const sf::Vector2f& offset, bool autoAssignID = true)
			: Bounds(offset, size)
		{
			if (autoAssignID) Init();
		}

		// collision with other colliders
		std::pair<bool, sf::FloatRect> Colliding(BoxCollider& other);
		std::pair<bool, sf::FloatRect> Colliding(CircleCollider& other);
		
		// collision with raw geometry
		std::pair<bool, sf::FloatRect> Colliding(const sf::Vector2f& point);
		std::pair<bool, sf::FloatRect> Colliding(const sf::Vector2f& centre, float radius);
		std::pair<bool, sf::FloatRect> Colliding(const sf::FloatRect& rect);

		sf::FloatRect GetLocalBounds() const override { return Bounds; }

		void DrawDebug(const sf::Transform& transform);
	};

}
