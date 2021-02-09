#include "Tilemap.h"


namespace SFMLEngine {


	Tilemap::Tilemap()
		: TileSize(), PaletteHandle(NULL_RESOURCE_ID), Tiles(), Geometry(sf::Triangles)
	{}

	Tilemap::Tilemap(ResourceID tilePalette, std::initializer_list<Tile> tiles)
		: PaletteHandle(tilePalette), Tiles(tiles), Geometry(sf::Triangles)
	{
		PalettePtr = ResourceManager::GetResourceHandle<TilePalette>(PaletteHandle);
		TileSize = (sf::Vector2f)PalettePtr->GetTileSize();

		BuildInitialGeometry();
	}

	void Tilemap::BuildInitialGeometry()
	{
		Geometry.clear();
		Geometry.resize(6 * Tiles.size());

		TilePalette* tilePalette = ResourceManager::GetResourceHandle<TilePalette>(PaletteHandle);

		for (const Tile& tile : Tiles)
		{
			// place a tile into the geometry
			// this can be done by inserting 2 triangles

			sf::Vector2f pos = sf::Vector2f(tile.Position.x * TileSize.x, tile.Position.y * TileSize.y);

			// first set the position of each vertex
			Geometry[m_TriangleIndex].position = pos;
			Geometry[m_TriangleIndex + 1].position = pos + sf::Vector2f(TileSize.x, 0);
			Geometry[m_TriangleIndex + 2].position = pos + TileSize;

			Geometry[m_TriangleIndex + 3].position = pos;
			Geometry[m_TriangleIndex + 4].position = pos + sf::Vector2f(0, TileSize.y);
			Geometry[m_TriangleIndex + 5].position = pos + TileSize;

			// then set the texture coordinates
			// we retrieve the location of the texture coordinates in the texture sheet from the tile palette
			sf::Vector2f texCoords = (sf::Vector2f)tilePalette->GetTexCoords(tile.TileType);
			Geometry[m_TriangleIndex].texCoords = texCoords;
			Geometry[m_TriangleIndex + 1].texCoords = texCoords + sf::Vector2f(TileSize.x, 0);
			Geometry[m_TriangleIndex + 2].texCoords = texCoords + TileSize;

			Geometry[m_TriangleIndex + 3].texCoords = texCoords;
			Geometry[m_TriangleIndex + 4].texCoords = texCoords + sf::Vector2f(0, TileSize.y);
			Geometry[m_TriangleIndex + 5].texCoords = texCoords + TileSize;

			m_TriangleIndex += 6;
		}
	}

	void Tilemap::PlaceTile(const sf::Vector2i& location, TileID tileType)
	{
		// add 6 more vertices into the geometry
		Geometry.resize(Geometry.getVertexCount() + 6);

		// get a pointer to the palette object
		TilePalette* tilePalette = ResourceManager::GetResourceHandle<TilePalette>(PaletteHandle);


		// create a tile object and add it into the tile map
		Tile tile{ tileType, location };
		Tiles.push_back(tile);

		// get the world coordinates of the tile
		sf::Vector2f pos = sf::Vector2f(tile.Position.x * TileSize.x, tile.Position.y * TileSize.y);


		// then add the new geometry

		// first set the position of each vertex
		Geometry[m_TriangleIndex].position = pos;
		Geometry[m_TriangleIndex + 1].position = pos + sf::Vector2f(TileSize.x, 0);
		Geometry[m_TriangleIndex + 2].position = pos + TileSize;

		Geometry[m_TriangleIndex + 3].position = pos;
		Geometry[m_TriangleIndex + 4].position = pos + sf::Vector2f(0, TileSize.y);
		Geometry[m_TriangleIndex + 5].position = pos + TileSize;

		// then set the texture coordinates
		// we retrieve the location of the texture coordinates in the texture sheet from the tile palette
		sf::Vector2f texCoords = (sf::Vector2f)tilePalette->GetTexCoords(tile.TileType);
		Geometry[m_TriangleIndex].texCoords = texCoords;
		Geometry[m_TriangleIndex + 1].texCoords = texCoords + sf::Vector2f(TileSize.x, 0);
		Geometry[m_TriangleIndex + 2].texCoords = texCoords + TileSize;

		Geometry[m_TriangleIndex + 3].texCoords = texCoords;
		Geometry[m_TriangleIndex + 4].texCoords = texCoords + sf::Vector2f(0, TileSize.y);
		Geometry[m_TriangleIndex + 5].texCoords = texCoords + TileSize;

		// increment the triangle index
		m_TriangleIndex += 6;
	}

}