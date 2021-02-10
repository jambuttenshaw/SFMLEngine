#pragma once

#include "ICollider.h"

#include <SFML/Graphics.hpp>


namespace SFMLEngine {

	struct BoxCollider;

	struct CircleCollider : public ICollider
	{
		float Radius;
		sf::Vector2f Offset;

		CircleCollider()
			: Radius(0), Offset()
		{
			Type = ColliderType::Circle;
		}
		CircleCollider(float radius, const sf::Vector2f& offset)
			: Radius(radius), Offset(offset)
		{
			Type = ColliderType::Circle;
		}


		bool Colliding(const CircleCollider& other) override;
		bool Colliding(const BoxCollider& other) override;

	};

}
