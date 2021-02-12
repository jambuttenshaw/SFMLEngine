#pragma once

#include <SFML/Graphics.hpp>

#include "../../System.h"
#include "CollisionData.h"


namespace SFMLEngine {

	struct CircleCollider;
	struct TilemapCollider;

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


		CollisionData Colliding(BoxCollider& other, const sf::Vector2f& otherPos);
		CollisionData Colliding(CircleCollider& other, const sf::Vector2f& otherPos);
		CollisionData Colliding(TilemapCollider& other, const sf::Vector2f& otherPos);

		const sf::Vector2f& GetBounds() const { return Size; }

		void DrawDebug();

	private:
		bool m_Modified = false;
	};

}
