#pragma once

#include <SFML/Graphics.hpp>

#include "../../Constants.h"
#include "../../Renderer/TilePalette.h"


namespace SFMLEngine {

	struct Tile
	{
		TileID TileType;
		sf::Vector2i Position;
		size_t GeometryIndex;

		Tile(TileID type, const sf::Vector2i& pos)
			: TileType(type), Position(pos), GeometryIndex(0)
		{}
	};

	struct Tilemap
	{
	public:
		friend class System;

		sf::Vector2f TileSize;
		
		ResourceID PaletteHandle;
		TilePalette* PalettePtr = nullptr;

		std::vector<Tile> Tiles;

		sf::VertexArray Geometry;

	public:
		Tilemap();
		Tilemap(ResourceID tilePalette);
		Tilemap(ResourceID tilePalette, std::initializer_list<Tile> tiles);

		void PlaceTile(const sf::Vector2i& location, TileID tileType);
		void RemoveTile(const sf::Vector2i& location);

		sf::Vector2i WorldToTileCoordinates(const sf::Vector2f& worldCoords);

	private:
		
		void AddTileToGeometry(Tile& tile);

		bool TileAtLocation(const sf::Vector2i& pos);
		size_t GetTileIndex(const sf::Vector2i& pos);

	private:
		bool m_Modified = false;

		size_t m_TriangleIndex = 0;
	};

}