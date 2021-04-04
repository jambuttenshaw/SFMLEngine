#pragma once

#include <SFML/Graphics.hpp>

#include "Collider.h"

#include "SFMLEngine/ECS/System.h"

/*
A BoxCollider is a rectangle that handles detecting overlaps with other geometry.
It contains a Rectangle that defines its size and local position.

It holds a pointer to a transform, which tells the BoxCollider how it is positioned in space.

BoxCollider is a Collider, and so it is automatically assigned a unique identifier to be used
internally in the collision system.

The main feature of a box collider (and indeed most colliders) is the overloads of the Colliding function,
which detects if this BoxCollider is intersecting with the geometry described the paramter or paramters passed in.
*/

namespace SFMLEngine {

	// forward-declare other types of collider, as to avoid circular includes
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

		void Reset(const sf::FloatRect& localBounds)
		{
			Bounds = localBounds;
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
