#include "TilemapCollider.h"

#include "BoxCollider.h"
#include "CircleCollider.h"

#include "SFMLEngine/Math.h"
#include "SFMLEngine/DebugTools.h"

#include "SFMLEngine/Systems/Physics/CollisionSystem.h"

#include <algorithm>

namespace SFMLEngine {

	void TilemapCollider::BuildCollisionGeometry()
	{
		ZoneScoped;

		// start with fresh geometry
		m_CollisionGeometry.clear();


		// create a quad for each tile in the tilemap
		for (auto& tile : TilemapHandle->Tiles)
		{
			// create a quad and add it to our collision geometry

			const sf::Vector2u& colliderSize = TilemapHandle->PalettePtr->GetColliderSize(tile.TileType);
			const sf::Vector2i& colliderOffset = TilemapHandle->PalettePtr->GetColliderOffset(tile.TileType);
			if (colliderSize.x == 0 || colliderSize.y == 0) continue; // dont create geometry for colliders that are missing a dimension

			// the size of the collider should be the size of the tile
			m_CollisionGeometry.push_back(
				BoxCollider{
					static_cast<sf::Vector2f>(colliderSize), // size
					{ (tile.Position.x * TilemapHandle->TileSize.x) + colliderOffset.x,
					  (tile.Position.y * TilemapHandle->TileSize.y) + colliderOffset.y }, // offset
					false // dont auto assign the id for the box collider
				});
		}

		if (Optimization > OptimizationLevel::None) 
		{
			ZoneScoped;
			ZoneName("Level1", 6);

			// then optimize out as many of those quads as possible
			bool optimal = false;
			std::vector<size_t> indicesToDelete;

			while (!optimal) {
				optimal = true;

				// we want to combine as many horizontally adjacent quads as we can
				size_t index = 0;
				for (auto& quad : m_CollisionGeometry)
				{
					// we dont want to try to optimize quads that are to be removed this pass
					if (std::find(indicesToDelete.begin(), indicesToDelete.end(), index) != indicesToDelete.end()) continue;

					size_t jIndex = 0;
					// look to see if there are any other quads at the same y level
					for (auto& secondQuad : m_CollisionGeometry)
					{
						if (std::find(indicesToDelete.begin(), indicesToDelete.end(), jIndex) != indicesToDelete.end()) continue;

						if (secondQuad.Bounds.top == quad.Bounds.top && secondQuad.Bounds != quad.Bounds)
						{
							// check to see if they are adjacent
							bool firstQuadLeftmost = quad.Bounds.left < secondQuad.Bounds.left;
							if (fabsf(quad.Bounds.left - secondQuad.Bounds.left) == (firstQuadLeftmost ? quad.Bounds.width : secondQuad.Bounds.width))
							{
								// they are adjacent!
								optimal = false;

								quad.Bounds.width += secondQuad.Bounds.width;
								if (!firstQuadLeftmost)
								{
									// second quad is to the left of the first quad
									// we also need to move the quad to the left by the width of the second quad
									quad.Bounds.left -= secondQuad.Bounds.width;
								}

								// set the second quad to be removed
								// add the index of the second quad to the set
								indicesToDelete.push_back(jIndex);
							}
						}
						jIndex++;
					}
					index++;
				}

				// delete any quads that were optimized out
				std::sort(indicesToDelete.begin(), indicesToDelete.end(), std::greater<size_t>());
				for (auto& i : indicesToDelete)
				{
					m_CollisionGeometry.erase(m_CollisionGeometry.begin() + i);
				}
				indicesToDelete.clear();
			}
		}

		if (Optimization > OptimizationLevel::Standard)
		{
			ZoneScoped;
			ZoneName("Level2", 6);

			// a second round of optimization
			bool optimal = false;
			std::vector<size_t> indicesToDelete;

			while (!optimal) {
				optimal = true;

				// we want to combine as many vertically adjacent quads of identical width as we can
				size_t index = 0;
				for (auto& quad : m_CollisionGeometry)
				{
					// we dont want to try to optimize quads that are to be removed this pass
					if (std::find(indicesToDelete.begin(), indicesToDelete.end(), index) != indicesToDelete.end()) continue;

					size_t jIndex = 0;
					// look to see if there are any other quads of the same width and horizontally aligned
					for (auto& secondQuad : m_CollisionGeometry)
					{
						if (std::find(indicesToDelete.begin(), indicesToDelete.end(), jIndex) != indicesToDelete.end()) continue;

						// quads must have the same x coordinate
						// they must be the same width
						// and they must not be the same quad
						if ((secondQuad.Bounds.left == quad.Bounds.left) && (secondQuad.Bounds.width == quad.Bounds.width) && (secondQuad.Bounds != quad.Bounds))
						{
							// check to see if they are adjacent
							bool firstQuadAbove = quad.Bounds.top < secondQuad.Bounds.top;
							if (fabsf(quad.Bounds.top - secondQuad.Bounds.top) == (firstQuadAbove ? quad.Bounds.height : secondQuad.Bounds.height))
							{
								// they are adjacent!
								optimal = false;

								quad.Bounds.height += secondQuad.Bounds.height;
								if (!firstQuadAbove)
								{
									// second quad is to the left of the first quad
									// we also need to move the quad to the left by the width of the second quad
									quad.Bounds.top -= secondQuad.Bounds.height;
								}

								// set the second quad to be removed
								// add the index of the second quad to the set
								indicesToDelete.push_back(jIndex);
							}
						}
						jIndex++;
					}
					index++;
				}

				// delete any quads that were optimized out
				std::sort(indicesToDelete.begin(), indicesToDelete.end(), std::greater<size_t>());
				for (auto& i : indicesToDelete)
				{
					m_CollisionGeometry.erase(m_CollisionGeometry.begin() + i);
				}
				indicesToDelete.clear();
			}
		}
		// we have a fully optimized tilemap collider  B-)

		{
			ZoneScoped;
			ZoneName("AssignID", 8);

			// give each collider an ID
			for (auto& i : m_CollisionGeometry)
			{
				i.SetNewColliderID();
				i.SetTransform(m_Transform);

				// if the tilemap collider is a trigger,
				// then so should all of its sub-colliders
				i.IsTrigger = IsTrigger;
			}
		}
	}

	void TilemapCollider::SetTransform(Transform* transform)
	{ 
		m_Transform = transform; 
		for (auto& collider : m_CollisionGeometry)
			collider.SetTransform(transform);
	}


	void TilemapCollider::FindBoundary()
	{
		sf::Vector2f topLeft, bottomRight;
		for (auto const& quad : m_CollisionGeometry)
		{
			topLeft = Math::Min(topLeft, sf::Vector2f(quad.Bounds.left, quad.Bounds.top));
			bottomRight = Math::Max(bottomRight, sf::Vector2f(quad.Bounds.left + quad.Bounds.width, quad.Bounds.top + quad.Bounds.height));
		}

		Size = bottomRight - topLeft;
	}

	void TilemapCollider::DrawDebug(const sf::Transform& transform)
	{
		for (auto const& rect : m_CollisionGeometry)
		{
			auto& transformed = transform.transformRect(rect.Bounds);
			DEBUG_DRAW_RECT(sf::Vector2f(transformed.left, transformed.top),
				sf::Vector2f(transformed.width, transformed.height),
				IsTrigger ? DebugTools::TRIGGER_COLOR : DebugTools::COLLIDER_COLOR);
		}
	}

}
