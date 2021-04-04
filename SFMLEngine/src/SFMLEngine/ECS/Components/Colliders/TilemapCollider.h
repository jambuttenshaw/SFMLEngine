#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/System.h"
#include "SFMLEngine/ECS/Components/Tilemap.h"
#include "Collider.h"


/*
Tilemap colliders are quite different from other colliders.

Instead of itself being an object that can be collided with, it instead holds a number of BoxColliders
that define its geometry. A tilemap collider automatically generates to match the shape of a tilemap assigned to it,
and an optimization level can be specified specify how many BoxColliders should be optimized out of the tilemap collider.

A higher optimization level means faster collision detection, but regenerating tilemap collider geometry takes significantly longer.
If you know the tilemap geometry will be edited infrequently then a high optimization level is ideal.
*/


namespace SFMLEngine {

	struct BoxCollider;
	struct CircleCollider;

	struct TilemapCollider : public Collider
	{
		// Specifies the level of optimization that should be applied to the collider
		enum class OptimizationLevel 
		{
			None = 0, Standard = 1, High = 2
		};


		Tilemap* TilemapHandle = nullptr;
		OptimizationLevel Optimization;
		sf::Vector2f Size;

		TilemapCollider()
			: Optimization(OptimizationLevel::None)
		{}
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
		void SetTransform(Transform* transform) override;

		void UpdateGeometry()
		{
			BuildCollisionGeometry();
			FindBoundary();
		}

		std::vector<BoxCollider>& GetCollisionGeometry() { return m_CollisionGeometry; }

		sf::FloatRect GetLocalBounds() const override { return sf::FloatRect(Size, sf::Vector2f{}); }

		void DrawDebug(const sf::Transform& transform);

	private:
		void BuildCollisionGeometry();
		void FindBoundary();

	private:
		std::vector<BoxCollider> m_CollisionGeometry;
	};

}
