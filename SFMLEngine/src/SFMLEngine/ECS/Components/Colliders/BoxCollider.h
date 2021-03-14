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

		std::pair<bool, sf::FloatRect> Colliding(BoxCollider& other);
		std::pair<bool, sf::FloatRect> Colliding(CircleCollider& other);

		sf::FloatRect GetLocalBounds() const override { return Bounds; }

		void DrawDebug(const sf::Transform& transform);
	};

}
