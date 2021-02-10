#pragma once

#include <SFML/Graphics.hpp>

#include "../../System.h"
#include "CollisionData.h"


namespace SFMLEngine {

	struct BoxCollider;

	struct CircleCollider
	{
		friend class System;

		float Radius;
		sf::Vector2f Offset;

		CircleCollider()
			: Radius(0), Offset()
		{}

		CircleCollider(float radius, const sf::Vector2f& offset)
			: Radius(radius), Offset(offset)
		{}


		CollisionData Colliding(const CircleCollider& other, const sf::Vector2f& otherPos);
		CollisionData Colliding(const BoxCollider& other, const sf::Vector2f& otherPos);

		const sf::Vector2f GetBounds() const { return 2.0f * sf::Vector2f(Radius, Radius); }

	private:
		bool m_Modified = false;
	};

}
