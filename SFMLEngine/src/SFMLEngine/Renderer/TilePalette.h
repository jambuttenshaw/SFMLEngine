#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/Constants.h"
#include "SFMLEngine/ResourceManagement/ResourceManager.h"


namespace SFMLEngine {


	// if the client loads a tilepalette from the same file multiple times,
	// it would make sense to just give them the ID of the existing one rather than
	// having multiple instances of the tile palette in memory

	// of course, there are cases where sharing a tile palette is undesirable
	// for example if the client wants to load a tile palette then make specific
	// modifications to it for one entity
	// so there is the option to load a tilepalette that is not for sharing
	struct TilePaletteCacheEntry
	{
		ResourceID PaletteID;
		std::string Filepath;
		bool Shared;
	};

	
	// this struct describes everything a tilemap could need to know about the tiles its using
	struct TileData
	{
		// a name to identify the tile (which is mostly for clients if they need to access a specific tile
		// from the palette)
		std::string Name;
		// a tile has the same texcoords for both the texture and normal map
		// just the image data is located in different images
		sf::Vector2u TexCoords;
		// the tile doesn't need to be the same size as the tile palettes defined size
		// but if a size for the individual tile is not specified then it defaults to the
		// palettes size
		sf::Vector2u TileSize;

		// tiles can have specific collider sizes and offests,
		// which can be specified in a palette json file
		// otherwise the offest defaults to 0, 0 and the size defaults to the tile size
		sf::Vector2i ColliderOffset;
		sf::Vector2u ColliderSize;
	};

	class TilePalette
	{
	private:
		// private constructors
		// use the ::Create or ::LoadFromFile methods to create tile palettes

		// creates an empty tile palette ready for tiles to be added to it
		TilePalette(const sf::Vector2u& tileSize);
		// creates a tile palette from textures, where the textures are split into a grid by the tileSize and each grid cell
		// is registered as a tile
		TilePalette(const std::string& texturePath, const std::string& normalMapPath, const sf::Vector2u& tileSize);
		// the most helpful way to create a tile palette is from a json file that loads a tile palette by reading in the data
		// about each tile from a json file
		TilePalette(const std::string& jsonPath);

	public:
		~TilePalette();

		// creates a new tile and adds it to the tile palette
		TileID CreateTile(const std::string& name, ResourceID tileTextureID, ResourceID tileNormalsID);

		// gets information about each type of tile
		const sf::Vector2u& GetTileSize() { return m_TileSize; }
		const TileData& GetTileData(TileID tile);
		const sf::Vector2u& GetTileSize(TileID tile);
		const sf::Vector2u& GetTexCoords(TileID tile);
		const sf::Vector2u& GetColliderSize(TileID tile);
		const sf::Vector2i& GetColliderOffset(TileID tile);

		// get the textures that the palette uses to render the tiles
		ResourceID GetTexture() { return m_PaletteTextureID; }
		ResourceID GetNormalMap() { return m_NormalMapTextureID; }

		// get information about what tiles are held in the palette
		size_t GetTileCount() { return m_TileCount; }
		TileID GetTileByName(const std::string& name);
		bool TileExists(const std::string& name);
		std::vector<TileID> GetAllTiles();

	private:
		// gets the next tile id in the queue to assign to a tile
		TileID GetNextTileID();

	public:
		// static methods

		// creates an empty tile palette
		static ResourceID Create(const sf::Vector2u& tileSize);
		// load a tile palette from textures
		static ResourceID LoadFromFile(const std::string& texturePath, const std::string& normalMapPath, const sf::Vector2u& tileSize, bool shared = true);
		// load a tile palette from a json metadata file
		static ResourceID LoadFromFile(const std::string& metaPath, bool shared = true);

		// destroy a tile palette when it is no longer needed
		static void Destroy(ResourceID palette);
		static void DestroyAllCached();

	private:

		// check if a tile palette has been loaded before
		static ResourceID PaletteCached(const std::string& path);

	private:
		// the general size of the tiles in the palette
		// individual tiles can have different sizes, but this is the default size
		sf::Vector2u m_TileSize;

		// the ID's of the textures that the tile palette uses
		ResourceID m_PaletteTextureID;
		ResourceID m_NormalMapTextureID;

		// a map that contains all of the tiles
		std::unordered_map<TileID, TileData> m_TileAtlas;

		// these values determine where to next place a tile in the tile palette texture
		// to keep the texture in a densely-packed square shape
		// not using a vector since we normally operate on these values individually
		unsigned int m_PlaceX = 0, m_PlaceY = 0;
		unsigned int m_MaxPlaceX = 0, m_MaxPlaceY = 0;


		// managing what tiles are created
		std::queue<TileID> m_AvailableTileIDs;
		size_t m_TileCount = 0;

	private:
		// a cache containing data about what tile palettes have been loaded
		static std::vector<TilePaletteCacheEntry> s_PaletteCache;
	};

}
