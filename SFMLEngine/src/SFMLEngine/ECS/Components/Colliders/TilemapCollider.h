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
		enum class OptimizationLevel 
		{
			None = 0, Standard = 1, High = 2
		};


		Tilemap* TilemapHandle = nullptr;
		OptimizationLevel Optimization;
		sf::Vector2f Size;

		TilemapCollider()
			: Optimization(OptimizationLevel::None)
		{
			Init();
		}
		TilemapCollider(OptimizationLevel optimize)
			: Optimization(optimize)
		{
			Init();
		}

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
		
		std::vector<sf::FloatRect> Colliding(TilemapCollider& other);
		std::vector<sf::FloatRect> Colliding(BoxCollider& other);
		std::vector<sf::FloatRect> Colliding(CircleCollider& other);

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
