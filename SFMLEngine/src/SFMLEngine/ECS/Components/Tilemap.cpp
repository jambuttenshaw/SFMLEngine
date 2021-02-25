#include "Tilemap.h"


namespace SFMLEngine {


	Tilemap::Tilemap()
		: TileSize(), PaletteHandle(NULL_RESOURCE_ID), Tiles(), Geometry(sf::Triangles)
	{}

	Tilemap::Tilemap(ResourceID tilePalette)
		: PaletteHandle(tilePalette), Tiles(), Geometry(sf::Triangles)
	{
		PalettePtr = ResourceManager::GetResourceHandle<TilePalette>(PaletteHandle);
		TileSize = (sf::Vector2f)PalettePtr->GetTileSize();
	}

	Tilemap::Tilemap(ResourceID tilePalette, std::initializer_list<Tile> tiles)
		: PaletteHandle(tilePalette), Tiles(tiles), Geometry(sf::Triangles)
	{
		PalettePtr = ResourceManager::GetResourceHandle<TilePalette>(PaletteHandle);
		TileSize = (sf::Vector2f)PalettePtr->GetTileSize();

		// set up the geometry to the correct size
		Geometry.clear();
		Geometry.resize(6 * Tiles.size());

		// add any tiles specified from the constructor
		for (Tile& tile : Tiles)
		{
			AddTileToGeometry(tile);
		}
	}

	void Tilemap::PlaceTile(const sf::Vector2i& location, TileID tileType, bool overwrite)
	{
		// do a check to make sure there isnt already a tile at that location
		if (TileAtLocation(location) && !overwrite)
		{
			LOG_CORE_INFO("Couldn't place tile: Tile at already at {0}, {1} and overwrite is false.", location.x, location.y);
			return;
		}

		// add 6 more vertices into the geometry
		Geometry.resize(Geometry.getVertexCount() + 6);

		// create a tile object and add it into the tile map
		Tile tile{ tileType, location };
		Tiles.push_back(tile);

		// create new vertices for this tile
		AddTileToGeometry(tile);


		// flag this component as modified
		m_Modified = true;
	}

	void Tilemap::RemoveTile(const sf::Vector2i& location)
	{
		if (!TileAtLocation(location)) return;

		// find out where the tile to be removed is in the array
		size_t tileIndex = GetTileIndex(location);

		// we dont need to move any data about if were deleting the last tile in the array
		if (tileIndex != Tiles.size() - 1)
		{
			// we need to move the last tile in the array into the empty gap left by the deleted one
			size_t geometryLocation = Tiles[tileIndex].GeometryIndex;

			// we want to replace the deleted tile with the last tile in the geometry
			// since all tiles are in the same order in the Tiles vector as in the geometry
			// we can just access the last element of the tiles vector and move it into the 
			// newly freed space

			// copy the last item into the empty space
			Tiles[tileIndex] = Tiles[Tiles.size() - 1];
			// remove the last item in the tiles array
			Tiles.pop_back();

			// now we want to copy the geometry data over to the new location
			size_t oldGeometryLocation = Tiles[tileIndex].GeometryIndex;

			Geometry[geometryLocation] = Geometry[oldGeometryLocation];
			Geometry[geometryLocation + 1] = Geometry[oldGeometryLocation + 1];
			Geometry[geometryLocation + 2] = Geometry[oldGeometryLocation + 2];
			Geometry[geometryLocation + 3] = Geometry[oldGeometryLocation + 3];
			Geometry[geometryLocation + 4] = Geometry[oldGeometryLocation + 4];
			Geometry[geometryLocation + 5] = Geometry[oldGeometryLocation + 5];

			// update the geometry location of the moved tile
			Tiles[tileIndex].GeometryIndex = geometryLocation;
		}
		else
		{
			// we have to remove the last tile from the tiles vector
			Tiles.pop_back();
		}

		// finally resize the geometry vertex array
		Geometry.resize(6 * Tiles.size());


		// flag this component as modified
		m_Modified = true;
	}


	sf::Vector2i Tilemap::WorldToTileCoordinates(const sf::Vector2f& worldCoords)
	{
		if (m_Transform)
		{
			auto pos = m_Transform->GetWorldToLocalTransformMatrix().transformPoint(worldCoords);
			return sf::Vector2i((int)floorf(pos.x / (float)TileSize.x),
								(int)floorf(pos.y / (float)TileSize.y));
		}
		else
		{
			return sf::Vector2i((int)floorf(worldCoords.x / (float)TileSize.x),
								(int)floorf(worldCoords.y / (float)TileSize.y));
		}
	}


	void Tilemap::AddTileToGeometry(Tile& tile)
	{
		// update the tile with the location in the geometry it has been inserted
		Tiles[Tiles.size() - 1].GeometryIndex = m_TriangleIndex;

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
		sf::Vector2f texCoords = (sf::Vector2f)PalettePtr->GetTexCoords(tile.TileType);
		Geometry[m_TriangleIndex].texCoords = texCoords;
		Geometry[m_TriangleIndex + 1].texCoords = texCoords + sf::Vector2f(TileSize.x, 0);
		Geometry[m_TriangleIndex + 2].texCoords = texCoords + TileSize;

		Geometry[m_TriangleIndex + 3].texCoords = texCoords;
		Geometry[m_TriangleIndex + 4].texCoords = texCoords + sf::Vector2f(0, TileSize.y);
		Geometry[m_TriangleIndex + 5].texCoords = texCoords + TileSize;

		m_TriangleIndex += 6;
	}

	bool Tilemap::TileAtLocation(const sf::Vector2i& pos)
	{
		for (auto const& tile : Tiles)
		{
			if (tile.Position == pos) return true;
		}
		return false;
	}

	size_t Tilemap::GetTileIndex(const sf::Vector2i& pos)
	{
		int index = 0;
		for (auto const& tile : Tiles)
		{
			if (tile.Position == pos) return index;
			index++;
		}
		return -1;
	}

}