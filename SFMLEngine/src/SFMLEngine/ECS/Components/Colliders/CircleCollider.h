#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/System.h"
#include "Collider.h"

/*
A CircleCollider is a circle defined by a centre and a radius that can detect overlaps between
other geometry in the game world.

It holds a pointer to a transform, which tells the CircleCollider how it is positioned in space.

CircleCollider is a Collider, and so it is automatically assigned a unique identifier to be used
internally in the collision system.

The main feature of a CircleCollider (and indeed most colliders) is the overloads of the Colliding function,
which detects if this CircleCollider is intersecting with the geometry described the paramter or paramters passed in.
*/

namespace SFMLEngine {

	struct BoxCollider;
	struct TilemapCollider;

	struct CircleCollider : public Collider
	{
		CircleCollider()
			: m_Radius(0), m_Offset()
		{}

		CircleCollider(float radius, const sf::Vector2f& offset)
			: m_Radius(radius), m_Offset(offset)
		{
			Init();
		}

		// collision with other colliders
		std::pair<bool, sf::FloatRect> Colliding(CircleCollider& other);
		std::pair<bool, sf::FloatRect> Colliding(BoxCollider& other);

		// collision with other geometry
		std::pair<bool, sf::FloatRect> Colliding(const sf::Vector2f& point);
		std::pair<bool, sf::FloatRect> Colliding(const sf::Vector2f& centre, float radius);
		std::pair<bool, sf::FloatRect> Colliding(const sf::FloatRect& rect);

		inline sf::FloatRect GetLocalBounds() const override { return sf::FloatRect{ m_Offset, 2.0f * sf::Vector2f(m_Radius, m_Radius) }; }

		inline void DrawDebug() {} // cant draw cirlces atm


	private:
		float m_Radius;
		sf::Vector2f m_Offset;
	};

}
