#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/System.h"
#include "Collider.h"

namespace SFMLEngine {

	struct BoxCollider;
	struct TilemapCollider;

	struct CircleCollider : public Collider
	{
		float Radius;
		sf::Vector2f Offset;

		CircleCollider()
			: Radius(0), Offset()
		{}

		CircleCollider(float radius, const sf::Vector2f& offset)
			: Radius(radius), Offset(offset)
		{
			Init();
		}


		std::pair<bool, sf::FloatRect> Colliding(CircleCollider& other);
		std::pair<bool, sf::FloatRect> Colliding(BoxCollider& other);

		sf::FloatRect GetLocalBounds() const override { return sf::FloatRect{ Offset, 2.0f * sf::Vector2f(Radius, Radius) }; }

		void DrawDebug() {} // cant draw cirlces atm
	};

}
