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
			// we want to deal with coordinates in tilemap space
			// at the moment, to make the logic as simple as possible
			m_CollisionGeometry.push_back(SubCollider{ NULL_COLLIDER_ID,
				sf::FloatRect { sf::Vector2f(tile.Position.x * TilemapHandle->TileSize.x, tile.Position.y * TilemapHandle->TileSize.y),
								TilemapHandle->TileSize }
				});
		}

		if (Optimization > OptimizationLevel::None) 
		{
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

						if (secondQuad.Rect.top == quad.Rect.top && secondQuad.Rect != quad.Rect)
						{
							// check to see if they are adjacent
							bool firstQuadLeftmost = quad.Rect.left < secondQuad.Rect.left;
							if (fabsf(quad.Rect.left - secondQuad.Rect.left) == (firstQuadLeftmost ? quad.Rect.width : secondQuad.Rect.width))
							{
								// they are adjacent!
								optimal = false;

								quad.Rect.width += secondQuad.Rect.width;
								if (!firstQuadLeftmost)
								{
									// second quad is to the left of the first quad
									// we also need to move the quad to the left by the width of the second quad
									quad.Rect.left -= secondQuad.Rect.width;
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
						if ((secondQuad.Rect.left == quad.Rect.left) && (secondQuad.Rect.width == quad.Rect.width) && (secondQuad.Rect != quad.Rect))
						{
							// check to see if they are adjacent
							bool firstQuadAbove = quad.Rect.top < secondQuad.Rect.top;
							if (fabsf(quad.Rect.top - secondQuad.Rect.top) == (firstQuadAbove ? quad.Rect.height : secondQuad.Rect.height))
							{
								// they are adjacent!
								optimal = false;

								quad.Rect.height += secondQuad.Rect.height;
								if (!firstQuadAbove)
								{
									// second quad is to the left of the first quad
									// we also need to move the quad to the left by the width of the second quad
									quad.Rect.top -= secondQuad.Rect.height;
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

		// give each collider an ID
		for (auto& i : m_CollisionGeometry)
		{
			i.ID = CollisionSystem::GetNextColliderID();
		}
	}

	void TilemapCollider::FindBoundary()
	{
		sf::Vector2f topLeft, bottomRight;
		for (auto const& quad : m_CollisionGeometry)
		{
			topLeft = Math::Max(topLeft, sf::Vector2f(quad.Rect.left, quad.Rect.top));
			bottomRight = Math::Max(bottomRight, sf::Vector2f(quad.Rect.left + quad.Rect.width, quad.Rect.top + quad.Rect.height));
		}

		Size = bottomRight - topLeft;
	}

	std::vector<std::pair<ColliderID, sf::FloatRect>> TilemapCollider::Colliding(TilemapCollider& other)
	{
		// tilemap vs tilemap collision
		return std::vector<std::pair<ColliderID, sf::FloatRect>>();
	}

	std::vector<std::pair<ColliderID, sf::FloatRect>> TilemapCollider::Colliding(BoxCollider& other)
	{
		// tilemap vs box collider collision
		ZoneScoped;
		std::vector<std::pair<ColliderID, sf::FloatRect>> collisions;

		sf::FloatRect otherGlobalBounds = other.GetGlobalBounds();

		sf::FloatRect globalQuad;
		for (auto const& quad : m_CollisionGeometry)
		{
			globalQuad = m_Transform->GetLocalToWorldTransformMatrix().transformRect(quad.Rect);
			if (globalQuad.intersects(otherGlobalBounds))
			{
				collisions.push_back(std::make_pair(quad.ID, std::move(globalQuad)));
			}
		}
		return collisions;
	}

	std::vector<std::pair<ColliderID, sf::FloatRect>> TilemapCollider::Colliding(CircleCollider& other)
	{
		/*
		// tilemap vs circle collider collision
		ZoneScoped;

		sf::FloatRect otherGlobalBounds = other.GetGlobalBounds();

		bool collides = false;
		sf::FloatRect globalQuad;
		for (auto const& quad : m_CollisionGeometry)
		{
			globalQuad = m_Transform->GetLocalToWorldTransformMatrix().transformRect(quad);
			if (globalQuad.intersects(otherGlobalBounds))
			{
				collides = true;
				break;
			}
		}
		*/
		return std::vector<std::pair<ColliderID, sf::FloatRect>>();
	}

	void TilemapCollider::DrawDebug(const sf::Transform& transform)
	{
		for (auto const& rect : m_CollisionGeometry)
		{
			auto& transformed = transform.transformRect(rect.Rect);
			DEBUG_DRAW_RECT(sf::Vector2f(transformed.left, transformed.top), sf::Vector2f(transformed.width, transformed.height), sf::Color::Green);
		}
	}

}
