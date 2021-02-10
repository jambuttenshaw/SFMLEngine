#pragma once

#include <SFML/Graphics.hpp>

#include "../../System.h"


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


		bool Colliding(const CircleCollider& other, const sf::Vector2f& otherPos);
		bool Colliding(const BoxCollider& other, const sf::Vector2f& otherPos);

	private:
		bool m_Modified = false;
	};

}