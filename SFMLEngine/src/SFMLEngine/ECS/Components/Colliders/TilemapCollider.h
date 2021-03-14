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
