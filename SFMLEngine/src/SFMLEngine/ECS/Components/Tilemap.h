#pragma once

#include <SFML/Graphics.hpp>

#include "../../Constants.h"
#include "../../Renderer/TilePalette.h"


namespace SFMLEngine {

	struct Tile
	{
		TileID TileType;
		sf::Vector2i Position;
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

	private:
		
		void BuildInitialGeometry();

	private:
		bool m_Modified = false;

		size_t m_TriangleIndex = 0;
	};

}