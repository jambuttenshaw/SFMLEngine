#pragma once

#include <SFML/Graphics.hpp>

#include "Collider.h"

#include "SFMLEngine/ECS/System.h"


namespace SFMLEngine {

	struct CircleCollider;
	struct TilemapCollider;

	struct BoxCollider : public Collider
	{
		sf::Vector2f Size;
		sf::Vector2f Offset;

		BoxCollider()
			: Size(), Offset()
		{}
		BoxCollider(const sf::Vector2f& size, const sf::Vector2f& offset)
			: Size(size), Offset(offset)
		{
			Init();
		}

		std::pair<ColliderID, sf::FloatRect> Colliding(BoxCollider& other);
		std::pair<ColliderID, sf::FloatRect> Colliding(CircleCollider& other);
		std::pair<ColliderID, sf::FloatRect> Colliding(TilemapCollider& other);

		sf::FloatRect GetLocalBounds() const override { return sf::FloatRect(Offset, Size); }

		void DrawDebug(const sf::Transform& transform);
	};

}
