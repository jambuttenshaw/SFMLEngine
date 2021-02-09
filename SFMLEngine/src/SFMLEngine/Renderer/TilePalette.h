#pragma once

#include <SFML/Graphics.hpp>

#include "../Constants.h"
#include "../ResourceManagement/ResourceManager.h"

namespace SFMLEngine {

	struct TileData
	{
		std::string Name;
		// a tile has the same texcoords for both the texture and normal map
		// just the image data is located in different images
		sf::Vector2u TexCoords;
	};

	class TilePalette
	{
	public:
		TilePalette(const sf::Vector2u& tileSize);
		~TilePalette();

		TileID CreateTile(const std::string& name, ResourceID tileTextureID, ResourceID tileNormalsID);

		const sf::Vector2u& GetTexCoords(TileID tile);

		const sf::Vector2u& GetTileSize() { return m_TileSize; }

	private:
		TileID GetNextTileID();

		TileID GetTileByName(const std::string& name);

	public:
		// static methods
		static ResourceID Create(const sf::Vector2u& tileSize);

		static void Destroy(ResourceID palette);
		static void DestroyAllCached();

	private:
		sf::Vector2u m_TileSize;
		ResourceID m_PaletteTextureID;
		ResourceID m_NormalMapTextureID;
		std::unordered_map<TileID, TileData> m_TileAtlas;

		// not using a vector since we normally operate on these values individually
		unsigned int m_PlaceX = 0, m_PlaceY = 0;
		unsigned int m_MaxPlaceX = 0, m_MaxPlaceY = 0;


		// managing what tiles are created
		std::queue<TileID> m_AvailableTileIDs;
		size_t m_TileCount = 0;

	private:
		static std::vector<ResourceID> s_PaletteCache;
	};

}