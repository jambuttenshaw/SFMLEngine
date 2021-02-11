#include "TilemapCollider.h"

#include "BoxCollider.h"
#include "CircleCollider.h"

#include "../../../Math.h"

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
				sf::Vector2f((tile.Position.x + 1) * TilemapHandle->TileSize.x, (tile.Position.y + 1) * TilemapHandle->TileSize.y),
				TilemapHandle->TileSize
				});
		}
		
		
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
						int diff = static_cast<int>(secondQuad.left - quad.left);
						if (abs(diff) == quad.width)
						{
							// they are adjacent!
							optimal = false;

							quad.width += secondQuad.width;
							if (diff < 0)
							{
								// second quad is to the left of the first quad
								// we also need to move the quad to the left by 1 space
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

	void TilemapCollider::FindBoundary()
	{
		sf::Vector2f topLeft, bottomRight;
		for (auto const& quad : m_CollisionGeometry)
		{
			topLeft = Max(topLeft, sf::Vector2f(quad.left, quad.top));
			bottomRight = Max(bottomRight, sf::Vector2f(quad.left + quad.width, quad.top + quad.height));
		}

		Size = bottomRight - topLeft;
	}

	CollisionData TilemapCollider::Colliding(TilemapCollider& other, const sf::Vector2f& otherPos)
	{
		// tilemap vs tilemap collision
		return CollisionData{ false, sf::Vector2f() };
	}

	CollisionData TilemapCollider::Colliding(BoxCollider& other, const sf::Vector2f& otherPos)
	{
		// tilemap vs box collider collision
		ZoneScoped;

		sf::Vector2f otherOffsetPos = other.Size + otherPos;
		sf::FloatRect otherBoundingBox{ otherOffsetPos, other.Size };

		bool collides = false;
		for (auto const& quad : m_CollisionGeometry)
		{
			if (quad.intersects(otherBoundingBox))
			{
				collides = true;
				break;
			}
		}

		return CollisionData{ collides, Size };

	}

	CollisionData TilemapCollider::Colliding(CircleCollider& other, const sf::Vector2f& otherPos)
	{
		// tilemap vs circle collider collision
		return CollisionData{ false, sf::Vector2f() };
	}

}
