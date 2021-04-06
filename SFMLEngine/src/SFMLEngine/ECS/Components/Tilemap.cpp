#include "Tilemap.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <iomanip>


namespace SFMLEngine {

	bool Tilemap::s_WarnOnLoadFailure = true;

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

		// we dont need to regenerate tilemap geometry or anything on the first frame
		m_Modified = false;
	}


	Tilemap::Tilemap(ResourceID tilePalette, const std::string& dataPath)
		: PaletteHandle(tilePalette), Tiles(), Geometry(sf::Triangles)
	{
		PalettePtr = ResourceManager::GetResourceHandle<TilePalette>(PaletteHandle);
		TileSize = (sf::Vector2f)PalettePtr->GetTileSize();

		// set up the geometry
		Geometry.clear();


		// load tilemap data from the file
		std::ifstream infile(dataPath);
		if (!infile.good())
		{
			if (s_WarnOnLoadFailure)
				LOG_CORE_WARN("Failed to open file '{0}'. Creating empty tilemap.", dataPath);
		}
		else
		{
			nlohmann::json tilemapJson;
			infile >> tilemapJson;

			for (auto& element : tilemapJson)
			{
				PlaceTile({ element["x"], element["y"] }, element["type"]);
			}

			infile.close();
		}

		// we dont need to regenerate tilemap geometry or anything on the first frame
		m_Modified = false;
	}


	void Tilemap::PlaceTile(const sf::Vector2i& location, TileID tileType, bool overwrite)
	{
		// do a check to make sure there isnt already a tile at that location
		if (TileAtLocation(location))
		{
			if (overwrite)
			{
				// we want to find the tile at that location and replace the data with the data of the new tile
				size_t index = GetTileIndex(location);

				// get the new tile dimensions
				sf::Vector2f tileDimensions = (sf::Vector2f)PalettePtr->GetTileSize(tileType);
				// check if the old tile is the same size as this tile
				bool same_size = tileDimensions == (sf::Vector2f)PalettePtr->GetTileSize(Tiles[index].TileType);

				// get the position of the tile in world space
				sf::Vector2f pos = sf::Vector2f(Tiles[index].Position.x * TileSize.x, Tiles[index].Position.y * TileSize.y);

				// set the new tile type
				Tiles[index].TileType = tileType;

				// update the texture coordinates in the geometry
				size_t geomIndex = Tiles[index].GeometryIndex;
				sf::Vector2f texCoords = (sf::Vector2f)PalettePtr->GetTexCoords(tileType);

				if (!same_size)
				{

					// vertices need adjusting as the new tile is not the same size as the old one
					Geometry[geomIndex].position = pos;
					Geometry[geomIndex + 1].position = pos + sf::Vector2f(tileDimensions.x, 0);
					Geometry[geomIndex + 2].position = pos + tileDimensions;

					Geometry[geomIndex + 3].position = pos;
					Geometry[geomIndex + 4].position = pos + sf::Vector2f(0, tileDimensions.y);
					Geometry[geomIndex + 5].position = pos + tileDimensions;
				}

				Geometry[geomIndex].texCoords = texCoords;
				Geometry[geomIndex + 1].texCoords = texCoords + sf::Vector2f(tileDimensions.x, 0);
				Geometry[geomIndex + 2].texCoords = texCoords + tileDimensions;

				Geometry[geomIndex + 3].texCoords = texCoords;
				Geometry[geomIndex + 4].texCoords = texCoords + sf::Vector2f(0, tileDimensions.y);
				Geometry[geomIndex + 5].texCoords = texCoords + tileDimensions;
			}
			else
			{
				LOG_CORE_INFO("Couldn't place tile: Tile at already at {0}, {1} and overwrite is false.", location.x, location.y);
				return;
			}
		}
		else
		{
			// add 6 more vertices into the geometry
			Geometry.resize(Geometry.getVertexCount() + 6);

			// create a tile object and add it into the tile map
			Tile tile{ tileType, location };
			Tiles.push_back(tile);

			// create new vertices for this tile
			AddTileToGeometry(tile);
		}

		// flag this component as modified
		m_Modified = true;
	}

	TileID Tilemap::RemoveTile(const sf::Vector2i& location)
	{
		if (!TileAtLocation(location)) return NULL_TILE_ID;

		// find out where the tile to be removed is in the array
		size_t tileIndex = GetTileIndex(location);

		TileID tiletype = Tiles[tileIndex].TileType;

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
		// decrement the triangle index since the array has been shrunk
		m_TriangleIndex -= 6;

		// flag this component as modified
		m_Modified = true;

		return tiletype;
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


	bool Tilemap::Export(const std::string& exportPath)
	{
		// write all of the data in the tilemap into a file in json format

		// create an empty json structure
		nlohmann::json tilemapJson;

		
		for (auto const& tile : Tiles)
		{
			nlohmann::json tileJson;
			tileJson["type"] = tile.TileType;
			tileJson["x"] = tile.Position.x;
			tileJson["y"] = tile.Position.y;

			tilemapJson.push_back(tileJson);
		}

		std::string serialized = tilemapJson.dump();
		std::ofstream outfile(exportPath);
		outfile << tilemapJson << std::endl;
		outfile.close();

		return false;
	}


	void Tilemap::AddTileToGeometry(Tile& tile)
	{
		// update the tile with the location in the geometry it has been inserted
		Tiles[Tiles.size() - 1].GeometryIndex = m_TriangleIndex;

		// place a tile into the geometry
		// this can be done by inserting 2 triangles

		sf::Vector2f pos = sf::Vector2f(tile.Position.x * TileSize.x, tile.Position.y * TileSize.y);

		// get the dimensions of the tile
		// the tile can have different dimensions than the tile size of the tile map
		// but we still need the original TileSize for the whole tilemap for placing tiles into the grid at the correct location
		sf::Vector2f tileDimensions = (sf::Vector2f)PalettePtr->GetTileSize(tile.TileType);

		// first set the position of each vertex
		Geometry[m_TriangleIndex].position = pos;
		Geometry[m_TriangleIndex + 1].position = pos + sf::Vector2f(tileDimensions.x, 0);
		Geometry[m_TriangleIndex + 2].position = pos + tileDimensions;

		Geometry[m_TriangleIndex + 3].position = pos;
		Geometry[m_TriangleIndex + 4].position = pos + sf::Vector2f(0, tileDimensions.y);
		Geometry[m_TriangleIndex + 5].position = pos + tileDimensions;

		// then set the texture coordinates
		// we retrieve the location of the texture coordinates in the texture sheet from the tile palette
		sf::Vector2f texCoords = (sf::Vector2f)PalettePtr->GetTexCoords(tile.TileType);
		Geometry[m_TriangleIndex].texCoords = texCoords;
		Geometry[m_TriangleIndex + 1].texCoords = texCoords + sf::Vector2f(tileDimensions.x, 0);
		Geometry[m_TriangleIndex + 2].texCoords = texCoords + tileDimensions;

		Geometry[m_TriangleIndex + 3].texCoords = texCoords;
		Geometry[m_TriangleIndex + 4].texCoords = texCoords + sf::Vector2f(0, tileDimensions.y);
		Geometry[m_TriangleIndex + 5].texCoords = texCoords + tileDimensions;

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