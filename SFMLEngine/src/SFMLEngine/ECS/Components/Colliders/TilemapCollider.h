#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/System.h"
#include "SFMLEngine/ECS/Components/Tilemap.h"
#include "Collider.h"


namespace SFMLEngine {

	struct BoxCollider;
	struct CircleCollider;

	struct TilemapCollider : public Collider
	{
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
		
		std::pair<bool, sf::FloatRect> Colliding(TilemapCollider& other);
		std::pair<bool, sf::FloatRect> Colliding(BoxCollider& other);
		std::pair<bool, sf::FloatRect> Colliding(CircleCollider& other);

		sf::FloatRect GetLocalBounds() const override { return sf::FloatRect(Size, sf::Vector2f{}); }

		const std::vector<sf::FloatRect>& GetCollisionGeometry() { return m_CollisionGeometry; }

		void DrawDebug(const sf::Transform& transform);

	private:
		void BuildCollisionGeometry();
		void FindBoundary();

	private:
		std::vector<sf::FloatRect> m_CollisionGeometry;
	};

}
