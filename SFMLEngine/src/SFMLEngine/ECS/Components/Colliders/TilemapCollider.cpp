#include "TilemapCollider.h"

#include "BoxCollider.h"
#include "CircleCollider.h"

#include "SFMLEngine/Math.h"
#include "SFMLEngine/DebugTools.h"

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
			m_CollisionGeometry.push_back(sf::FloatRect{
				sf::Vector2f(tile.Position.x * TilemapHandle->TileSize.x, tile.Position.y * TilemapHandle->TileSize.y),
				TilemapHandle->TileSize
				});
		}

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

						if (secondQuad.top == quad.top && secondQuad != quad)
						{
							// check to see if they are adjacent
							bool firstQuadLeftmost = quad.left < secondQuad.left;
							if (fabsf(quad.left - secondQuad.left) == (firstQuadLeftmost ? quad.width : secondQuad.width))
							{
								// they are adjacent!
								optimal = false;

								quad.width += secondQuad.width;
								if (!firstQuadLeftmost)
								{
									// second quad is to the left of the first quad
									// we also need to move the quad to the left by the width of the second quad
									quad.left -= secondQuad.width;
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
						if ((secondQuad.left == quad.left) && (secondQuad.width == quad.width) && (secondQuad != quad))
						{
							// check to see if they are adjacent
							bool firstQuadAbove = quad.top < secondQuad.top;
							if (fabsf(quad.top - secondQuad.top) == (firstQuadAbove ? quad.height : secondQuad.height))
							{
								// they are adjacent!
								optimal = false;

								quad.height += secondQuad.height;
								if (!firstQuadAbove)
								{
									// second quad is to the left of the first quad
									// we also need to move the quad to the left by the width of the second quad
									quad.top -= secondQuad.height;
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
	}

	void TilemapCollider::FindBoundary()
	{
		sf::Vector2f topLeft, bottomRight;
		for (auto const& quad : m_CollisionGeometry)
		{
			topLeft = Math::Max(topLeft, sf::Vector2f(quad.left, quad.top));
			bottomRight = Math::Max(bottomRight, sf::Vector2f(quad.left + quad.width, quad.top + quad.height));
		}

		Size = bottomRight - topLeft;
	}

	std::pair<bool, sf::FloatRect> TilemapCollider::Colliding(TilemapCollider& other)
	{
		// tilemap vs tilemap collision
		return std::make_pair(false, GetGlobalBounds());
	}

	std::pair<bool, sf::FloatRect> TilemapCollider::Colliding(BoxCollider& other)
	{
		// tilemap vs box collider collision
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

		return std::make_pair(collides, globalQuad);
	}

	std::pair<bool, sf::FloatRect> TilemapCollider::Colliding(CircleCollider& other)
	{
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

		return std::make_pair(collides, globalQuad);
	}

	void TilemapCollider::DrawDebug(const sf::Transform& transform)
	{
		for (auto const& rect : m_CollisionGeometry)
		{
			auto& transformed = transform.transformRect(rect);
			DEBUG_DRAW_RECT(sf::Vector2f(transformed.left, transformed.top), sf::Vector2f(transformed.width, transformed.height), sf::Color::Green);
		}
	}

}
