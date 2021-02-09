#include "TilePalette.h"

#include "Texture.h"

namespace SFMLEngine {

	std::vector<ResourceID> TilePalette::s_PaletteCache;

	TilePalette::TilePalette(const sf::Vector2u& tileSize)
		: m_TileSize(tileSize), m_TileAtlas()
	{
		// by default create a palette that will hold 1 tile
		m_PaletteTextureID = Texture::Create((sf::Vector2u)m_TileSize);
		m_NormalMapTextureID = Texture::Create((sf::Vector2u)m_TileSize);
	}

	TilePalette::~TilePalette()
	{
		// destroy the textures
		Texture::Destroy(m_PaletteTextureID);
		Texture::Destroy(m_NormalMapTextureID);

		m_TileAtlas.clear();
	}

	TileID TilePalette::CreateTile(const std::string& name, ResourceID tileTextureID, ResourceID tileNormalsID)
	{
		// make sure we dont have a tile with the same name already
		SFMLE_CORE_ASSERT(GetTileByName(name) == NULL_TILE_ID, "A tile with that name already exists!");

		// get the resource handles for one of the textures so we know the palette size
		sf::Texture* palette = ResourceManager::GetResourceHandle<sf::Texture>(m_PaletteTextureID);

		// get the size of the palette
		sf::Vector2u paletteSize = palette->getSize();


		bool resize = false;
		// make sure there is enough space in the texture to place a new tile
		// if adding on tileSize from the placing point goes outside of the palette,
		// we need to resize the texture
		if (m_PlaceX > paletteSize.x - m_TileSize.x || m_PlaceY > paletteSize.y - m_TileSize.y)
		{
			// flag as having been resized
			resize = true;

			// there is not enough space, the textures need resized first
			// add on one tiles width and height from the current placing point
			// make sure to resize both textures
			m_PaletteTextureID = Texture::Resize(m_PaletteTextureID, sf::Vector2u(m_PlaceX + m_TileSize.x, m_PlaceY + m_TileSize.y));
			m_NormalMapTextureID = Texture::Resize(m_NormalMapTextureID, sf::Vector2u(m_PlaceX + m_TileSize.x, m_PlaceY + m_TileSize.y));
		}

		// now that the textures are the right size, retrieve handles to them
		// we only need to get a new handle to the palette if it was resized
		if (resize) palette = ResourceManager::GetResourceHandle<sf::Texture>(m_PaletteTextureID);
		sf::Texture* normalMap = ResourceManager::GetResourceHandle<sf::Texture>(m_NormalMapTextureID);


		// get the texture handles for the tile
		sf::Texture* tileTexture = ResourceManager::GetResourceHandle<sf::Texture>(tileTextureID);
		sf::Texture* tileNormals = ResourceManager::GetResourceHandle<sf::Texture>(tileNormalsID);

		// copy the image data into the palette and normal map textures
		palette->update(*tileTexture, m_PlaceX, m_PlaceY);
		normalMap->update(*tileNormals, m_PlaceX, m_PlaceY);


		// place the tile data into the tile atlas
		TileID newID = GetNextTileID();
		m_TileAtlas.insert(std::make_pair(newID, TileData{name, sf::Vector2u(m_PlaceX, m_PlaceY)} ));


		m_MaxPlaceX = std::max(m_MaxPlaceX, m_PlaceX);
		m_MaxPlaceY = std::max(m_MaxPlaceY, m_PlaceY);

		// work out the place to place the next tile
		// the goal of this is to keep the texture a square
		if (m_PlaceX == m_PlaceY)
		{
			m_PlaceX += m_TileSize.x;
			m_PlaceY = 0;
		}
		else if (m_MaxPlaceX > m_MaxPlaceY)
		{
			m_PlaceY += m_TileSize.y;
			if (m_PlaceY > m_MaxPlaceY) m_PlaceX = 0;
		}
		else
		{
			m_MaxPlaceX += m_TileSize.x;
		}


		// delete the individual tile textures
		Texture::Destroy(tileTextureID);
		Texture::Destroy(tileNormalsID);

		// finally return the ID of the tile created
		return newID;
	}

	const sf::Vector2u& TilePalette::GetTexCoords(TileID tile)
	{
		SFMLE_CORE_ASSERT(m_TileAtlas.find(tile) != m_TileAtlas.end(), "Tile has not been registered!");
		return m_TileAtlas[tile].TexCoords;
	}

	TileID TilePalette::GetNextTileID()
	{
		SFMLE_CORE_ASSERT(m_TileCount < MAX_TILES, "Too many tiles in this tilemap!");

		TileID nextID = m_AvailableTileIDs.front();
		m_AvailableTileIDs.pop();
		++m_TileCount;

		return nextID;
	}

	TileID TilePalette::GetTileByName(const std::string& name)
	{
		for (auto const& tile : m_TileAtlas)
		{
			if (tile.second.Name == name) return tile.first;
		}
		return NULL_TILE_ID;
	}




	ResourceID TilePalette::Create(const sf::Vector2u& tileSize)
	{
		TilePalette* newPalette = new TilePalette(tileSize);
		ResourceID newID = ResourceManager::ManageResource(newPalette);

		s_PaletteCache.push_back(newID);
		return newID;
	}

	void TilePalette::Destroy(ResourceID palette)
	{
		int index = 0;
		for (auto& p : s_PaletteCache)
		{
			if (p == palette)
			{
				ResourceManager::DeleteResource<TilePalette>(palette);
				s_PaletteCache.erase(s_PaletteCache.begin() + index);
				break;
			}
			++index;
		}

		LOG_CORE_WARN("Attempted to delete palette '{0}' but it did not exist.", palette);
	}

	void TilePalette::DestroyAllCached()
	{
		for (auto& p : s_PaletteCache)
		{
			ResourceManager::DeleteResource<TilePalette>(p);
		}
		s_PaletteCache.clear();
	}

}