#include "TilemapCollider.h"

#include "BoxCollider.h"
#include "CircleCollider.h"

#include "SFMLEngine/Math.h"
#include "SFMLEngine/DebugTools.h"

#include "SFMLEngine/Systems/Physics/CollisionSystem.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <iomanip>

#include <algorithm>

namespace SFMLEngine {


	TilemapCollider::TilemapCollider(const std::string& jsonFile)
	{
		Init();

		m_LoadedFromFile = true;


		// load data from the file
		std::ifstream infile(jsonFile);
		if (!infile.good())
		{
			LOG_CORE_WARN("Failed to open file '{0}'. Geometry will be regenerated.", jsonFile);
			// this will trigger a geometry regeneration
			m_LoadedFromFile = false;
		}
		else
		{
			nlohmann::json colliderJson;
			infile >> colliderJson;

			m_OptimizationLevel = static_cast<OptimizationLevel>(colliderJson["optimizationLevel"]);
			for (auto& box : colliderJson["geometry"])
			{
				m_CollisionGeometry.push_back(
					BoxCollider{
						{ box["w"], box["h"] }, // size
						{ box["x"], box["y"] }, // offset
						false // dont auto assign the id for the box collider, it will be done manually later
					});
			}

			infile.close();
		}


		// give each collider an ID
		for (auto& i : m_CollisionGeometry)
		{
			i.SetNewColliderID();
			// also gives each sub-collider a pointer to the transform
			i.SetTransform(m_Transform);

			// if the tilemap collider is a trigger,
			// then so should all of its sub-colliders
			i.SetTrigger(IsTrigger());
		}
	}


	void TilemapCollider::BuildCollisionGeometry()
	{
		ZoneScoped;


		/*
		The theory behind this optimization technique is that two rectangles that perfectly share an edge
		can be combined into one larger rectangle. This process can be executed horzintally, vertically or both.

		I introduced a limit to the width and height of the sub-colliders, so that a sub-collider can never be in more than 4
		spatial partitions at one time.
		*/


		// start with fresh geometry
		m_CollisionGeometry.clear();


		// create a quad for each tile in the tilemap
		for (auto& tile : m_TilemapHandle->GetTiles())
		{
			// create a quad and add it to our collision geometry

			// get the info about the collider for this tile from the tile palette
			const sf::Vector2u& colliderSize = m_TilemapHandle->GetPalette()->GetColliderSize(tile.TileType);
			const sf::Vector2i& colliderOffset = m_TilemapHandle->GetPalette()->GetColliderOffset(tile.TileType);

			// dont create geometry for colliders that are missing a dimension
			// this could be due to data input incorrectly, or could be deliberate to disable the collider for some
			// specific tiles
			if (colliderSize.x == 0 || colliderSize.y == 0) continue; 

			// add a new box collider into the collision geometry
			m_CollisionGeometry.push_back(
				BoxCollider{
					static_cast<sf::Vector2f>(colliderSize), // size
					{ (tile.Position.x * m_TilemapHandle->GetTileSize().x) + colliderOffset.x,
					  (tile.Position.y * m_TilemapHandle->GetTileSize().y) + colliderOffset.y }, // offset
					false // dont auto assign the id for the box collider, as it might get deleted later
				});
		}

		if (m_OptimizationLevel > OptimizationLevel::None) 
		{
			ZoneScoped;
			ZoneName("Level1", 6);

			// then optimize out as many of those quads as possible
			bool optimal = false;
			// since range-based for loops are in use, items need to be deleted after the loop has finished
			// so as not to invalidate iterators
			std::vector<size_t> indicesToDelete;

			// repeat until a pass happens where no more colliders are removed
			while (!optimal) {
				optimal = true;

				// we want to combine as many horizontally adjacent quads as we can
				size_t index = -1;
				for (auto& quad : m_CollisionGeometry)
				{
					// increment the index first so as we don't accidently 'continue' past it
					index++;


					// some checks to make sure we actually want to attempt to optimize this quad
					
					// if this quad has reached the max size then we do not want to add anymore tiles onto it
					if (quad.GetLocalBounds().width >= MAX_TILEMAP_COLLIDER_SIZE * m_TilemapHandle->GetTileSize().x) continue;


					// we dont want to try to optimize quads that are to be removed this pass
					if (std::find(indicesToDelete.begin(), indicesToDelete.end(), index) != indicesToDelete.end()) continue;

					size_t jIndex = -1;
					// look to see if there are any other quads at the same y level
					for (auto& secondQuad : m_CollisionGeometry)
					{
						jIndex++;

						// dont try to combine the quad with itself
						// thats nonsense
						if (secondQuad.GetLocalBounds() == quad.GetLocalBounds()) continue;

						// some checks to make sure we actually want to attempt to optimize this quad

						// make sure joining these quads will not make them too large
						if (secondQuad.GetLocalBounds().width + quad.GetLocalBounds().width > MAX_TILEMAP_COLLIDER_SIZE * m_TilemapHandle->GetTileSize().x) continue;
						// make sure this quad hasnt already been marked as deleted
						if (std::find(indicesToDelete.begin(), indicesToDelete.end(), jIndex) != indicesToDelete.end()) continue;


						// make sure this quad perfectly shares an edge with the second one
						if (secondQuad.GetLocalBounds().top == quad.GetLocalBounds().top && secondQuad.GetLocalBounds().height == quad.GetLocalBounds().height)
						{
							// we need to know which quad came first
							bool firstQuadLeftmost = quad.GetLocalBounds().left < secondQuad.GetLocalBounds().left;
							// check to see if they are adjacent
							if (fabsf(quad.GetLocalBounds().left - secondQuad.GetLocalBounds().left) == (firstQuadLeftmost ? quad.GetLocalBounds().width : secondQuad.GetLocalBounds().width))
							{
								// they are adjacent!
								optimal = false;

								// the new quad is as wide as the other 2 combined
								quad.SetWidth(quad.GetLocalBounds().width + secondQuad.GetLocalBounds().width);
								if (!firstQuadLeftmost)
								{
									// second quad is to the left of the first quad
									// we also need to move the quad to the left by the width of the second quad
									quad.SetLeft(quad.GetLocalBounds().left - secondQuad.GetLocalBounds().width);
								}

								// set the second quad to be removed
								// add the index of the second quad to the set
								indicesToDelete.push_back(jIndex);

								// check if this quad has now reached the max size
								if (quad.GetLocalBounds().width >= MAX_TILEMAP_COLLIDER_SIZE * m_TilemapHandle->GetTileSize().x) break;

							}
						}
					}
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

		// the process here is identical as before
		// but optimizing on the y axis instead of the x axis
		// effectively the same algorithm, but just switching x with y and width with height
		if (m_OptimizationLevel > OptimizationLevel::Standard)
		{
			ZoneScoped;
			ZoneName("Level2", 6);

			// a second round of optimization
			bool optimal = false;
			std::vector<size_t> indicesToDelete;

			while (!optimal) {
				optimal = true;

				// we want to combine as many vertically adjacent quads of identical width as we can
				size_t index = -1;
				for (auto& quad : m_CollisionGeometry)
				{
					index++;

					// if this quad has reached the max size then we do not want to add anymore tiles onto it
					if (quad.GetLocalBounds().height >= MAX_TILEMAP_COLLIDER_SIZE * m_TilemapHandle->GetTileSize().y) continue;


					// we dont want to try to optimize quads that are to be removed this pass
					if (std::find(indicesToDelete.begin(), indicesToDelete.end(), index) != indicesToDelete.end()) continue;

					size_t jIndex = -1;
					// look to see if there are any other quads of the same width and horizontally aligned
					for (auto& secondQuad : m_CollisionGeometry)
					{
						jIndex++;

						// make sure joining these quads will not make them too large
						if (secondQuad.GetLocalBounds().height + quad.GetLocalBounds().height > MAX_TILEMAP_COLLIDER_SIZE * m_TilemapHandle->GetTileSize().y) continue;


						if (std::find(indicesToDelete.begin(), indicesToDelete.end(), jIndex) != indicesToDelete.end()) continue;

						// quads must have the same x coordinate
						// they must be the same width
						// and they must not be the same quad
						if ((secondQuad.GetLocalBounds().left == quad.GetLocalBounds().left) && (secondQuad.GetLocalBounds().width == quad.GetLocalBounds().width) && (secondQuad.GetLocalBounds() != quad.GetLocalBounds()))
						{
							// check to see if they are adjacent
							bool firstQuadAbove = quad.GetLocalBounds().top < secondQuad.GetLocalBounds().top;
							if (fabsf(quad.GetLocalBounds().top - secondQuad.GetLocalBounds().top) == (firstQuadAbove ? quad.GetLocalBounds().height : secondQuad.GetLocalBounds().height))
							{
								// they are adjacent!
								optimal = false;

								quad.SetHeight(quad.GetLocalBounds().height + secondQuad.GetLocalBounds().height);
								if (!firstQuadAbove)
								{
									// second quad is to the left of the first quad
									// we also need to move the quad to the left by the width of the second quad
									quad.SetTop(quad.GetLocalBounds().top - secondQuad.GetLocalBounds().height);
								}

								// set the second quad to be removed
								// add the index of the second quad to the set
								indicesToDelete.push_back(jIndex);

								// check if this quad has now reached the max size
								if (quad.GetLocalBounds().height >= MAX_TILEMAP_COLLIDER_SIZE * m_TilemapHandle->GetTileSize().y) break;
							}
						}
					}
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
				// also gives each sub-collider a pointer to the transform
				i.SetTransform(m_Transform);

				// if the tilemap collider is a trigger,
				// then so should all of its sub-colliders
				i.SetTrigger(IsTrigger());
			}
		}
	}


	void TilemapCollider::SetTransform(Transform* transform)
	{ 
		// overload of Collider::SetTransform
		// because a tilemap collider needs to set all of the sub-colliders transforms too
		m_Transform = transform; 
		for (auto& collider : m_CollisionGeometry)
			collider.SetTransform(transform);
	}


	void TilemapCollider::FindBoundary()
	{
		// sets Size to be the rect encompassing every single sub-collider in the tilemap
		sf::Vector2f topLeft, bottomRight;
		for (auto const& quad : m_CollisionGeometry)
		{
			topLeft = Math::Min(topLeft, sf::Vector2f(quad.GetLocalBounds().left, quad.GetLocalBounds().top));
			bottomRight = Math::Max(bottomRight, sf::Vector2f(quad.GetLocalBounds().left + quad.GetLocalBounds().width,
															  quad.GetLocalBounds().top + quad.GetLocalBounds().height));
		}

		// the size is the distance from the very topleft to the very bottomright
		m_Size = bottomRight - topLeft;
	}

	void TilemapCollider::DrawDebug(const sf::Transform& transform)
	{
		// draws every rect in the tilemap collider for debugging purposes
		for (auto const& rect : m_CollisionGeometry)
		{
			auto& transformed = transform.transformRect(rect.GetLocalBounds());
			DEBUG_DRAW_RECT(sf::Vector2f(transformed.left, transformed.top),
				sf::Vector2f(transformed.width, transformed.height),
				IsTrigger() ? DebugTools::TRIGGER_COLOR : DebugTools::COLLIDER_COLOR);
		}
	}

	void TilemapCollider::Export(const std::string& filepath)
	{
		// write all of the collider data into a json file

		// create an empty json object
		nlohmann::json colliderJson;
		colliderJson["optimizationLevel"] = static_cast<int>(m_OptimizationLevel);

		colliderJson["geometry"] = nlohmann::json::array();
		for (auto const& box : m_CollisionGeometry)
		{
			nlohmann::json boxJson;
			sf::FloatRect rect{ box.GetLocalBounds() };
			boxJson["x"] = rect.left;
			boxJson["y"] = rect.top;
			boxJson["w"] = rect.width;
			boxJson["h"] = rect.height;

			colliderJson["geometry"].push_back(boxJson);
		}

		std::string serialized = colliderJson.dump();
		std::ofstream outfile(filepath);
		outfile << serialized << std::endl;
		outfile.close();
	}

}
