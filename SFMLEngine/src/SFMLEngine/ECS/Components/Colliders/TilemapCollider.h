#pragma once

#include <SFML/Graphics.hpp>

#include "../../System.h"
#include "../Tilemap.h"
#include "CollisionData.h"


namespace SFMLEngine {

	struct BoxCollider;
	struct CircleCollider;

	struct TilemapCollider
	{
		friend class System;

		Tilemap* TilemapHandle = nullptr;

		sf::Vector2f Size;

		TilemapCollider()
		{}

		void Setup(Tilemap* tilemapHandle)
		{
			TilemapHandle = tilemapHandle;
			UpdateGeometry();
		}

		void UpdateGeometry()
		{
			BuildCollisionGeometry();
			FindBoundary();
		}
		
		CollisionData Colliding(TilemapCollider& other, const sf::Vector2f& otherPos);
		CollisionData Colliding(BoxCollider& other, const sf::Vector2f& otherPos);
		CollisionData Colliding(CircleCollider& other, const sf::Vector2f& otherPos);

		const sf::Vector2f& GetBounds() const { return Size; }
		const std::vector<sf::FloatRect>& GetCollisionGeometry() { return m_CollisionGeometry; }

		void DrawDebug(const sf::Transform& transform);

	private:
		void BuildCollisionGeometry();
		void FindBoundary();

	private:
		bool m_Modified = false;

		std::vector<sf::FloatRect> m_CollisionGeometry;
	};

}
