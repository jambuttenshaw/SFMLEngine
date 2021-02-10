#pragma once

#include <SFML/Graphics.hpp>

#include "../../System.h"


namespace SFMLEngine {

	struct CircleCollider;

	struct BoxCollider
	{
		friend class System;

		sf::Vector2f Size;
		sf::Vector2f Offset;

		BoxCollider()
			: Size(), Offset()
		{}
		BoxCollider(const sf::Vector2f& size, const sf::Vector2f& offset)
			: Size(size), Offset(offset)
		{}


		bool Colliding(const BoxCollider& other, const sf::Vector2f& otherPos);
		bool Colliding(const CircleCollider& other, const sf::Vector2f& otherPos);

	private:
		bool m_Modified = false;
	};

}