#pragma once

#include "ICollider.h"

#include <SFML/Graphics.hpp>


namespace SFMLEngine {

	struct CircleCollider;

	struct BoxCollider : public ICollider
	{
		sf::Vector2f Size;
		sf::Vector2f Offset;

		BoxCollider()
			: Size(), Offset()
		{
			Type = ColliderType::Box;
		}
		BoxCollider(const sf::Vector2f& size, const sf::Vector2f& offset)
			: Size(size), Offset(offset)
		{
			Type = ColliderType::Box;
		}


		bool Colliding(const BoxCollider& other);
		bool Colliding(const CircleCollider& other);

	};

}
