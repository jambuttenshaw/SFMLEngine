#include "TilePalette.h"

#include "Texture.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <iomanip>


namespace SFMLEngine {

	std::vector<TilePaletteCacheEntry> TilePalette::s_PaletteCache;

	TilePalette::TilePalette(const sf::Vector2u& tileSize)
		: m_TileSize(tileSize), m_TileAtlas()
	{
		// by default create a palette that will hold 1 tile
		m_PaletteTextureID = Texture::Create((sf::Vector2u)m_TileSize);
		m_NormalMapTextureID = Texture::Create((sf::Vector2u)m_TileSize);


		// create the queue for tile IDs
		for (TileID i = 0; i < MAX_TILES; i++)
		{
			m_AvailableTileIDs.push(i);
		}
	}

	TilePalette::TilePalette(const std::string& texturePath, const std::string& normalMapPath, const sf::Vector2u& tileSize)
		: m_TileSize(tileSize), m_TileAtlas()
	{
		// load the textures from their files
		m_PaletteTextureID = Texture::Create(texturePath);
		m_NormalMapTextureID = Texture::Create(normalMapPath);

		// create the queue for tile IDs
		for (TileID i = 0; i < MAX_TILES; i++)
		{
			m_AvailableTileIDs.push(i);
		}


		// we need to add all of the tiles currently existing in the texture to the tile atlas
		// this is done by splitting the texture into a grid by the tile size and registering each cell
		// as a different tile

		// where tiles may have differing sizes, or they are not placed in a grid in the texture,
		// a json file should be used to specify how the tile palette is created

		auto texPtr = ResourceManager::GetResourceHandle<sf::Texture>(m_PaletteTextureID);
		auto const& dims = texPtr->getSize();

		// get the number of tiles currently in the spritesheet
		unsigned int numTilesX = dims.x / m_TileSize.x;
		unsigned int numTilesY = dims.y / m_TileSize.y;

		SFMLE_CORE_ASSERT(numTilesX > 0 && numTilesY > 0, "Invalid tile palette texture!");

		for (unsigned int x = 0; x < numTilesX; x++)
		{
			for (unsigned int y = 0; y < numTilesY; y++)
			{
				// place the tile data into the tile atlas
				TileID newID = GetNextTileID();
				m_TileAtlas.insert(std::make_pair(newID, TileData{ "Tile" + std::to_string(y * numTilesX + x), sf::Vector2u(x * m_TileSize.x, y * m_TileSize.y), m_TileSize }));
			}
		}
		// assign the place to put more tiles in case more are added by scripts later
		m_PlaceX = (numTilesX + 1) * m_TileSize.x;
		m_PlaceY = 0;
	}


	TilePalette::TilePalette(const std::string& jsonPath)
	{
		// create the queue for tile IDs
		for (TileID i = 0; i < MAX_TILES; i++)
		{
			m_AvailableTileIDs.push(i);
		}


		// load the json data describing how to construct our tile palette
		// load tilemap data from the file
		std::ifstream infile(jsonPath);
		if (!infile.good())
		{
			// make sure we can actually open the file
			// if something went wrong then this is a crash
			LOG_CORE_ERROR("Failed to open file '{0}'", jsonPath);
			SFMLE_CORE_ASSERT(0, "Error opening file");
		}

		// create a json object to hold the data about our palette
		nlohmann::json paletteJson;
		try
		{
			// deserialize the data file into the json object
			infile >> paletteJson;
		}
		catch (const std::exception& e)
		{
			// if something went wrong, then there is likely a syntax error in the json file
			// log the exception message to try help the user solve their issues
			LOG_CORE_ERROR("Exception occurred: {0}", e.what());
			SFMLE_CORE_ASSERT(0, "Failed to parse palette JSON.");
		}
		

		// we use a try clause so that we can output more helpful information
		// in the event that something goes wrong when trying to retrieve data from the json file
		try
		{

			// now we can start loading up the tilemap


			// create textures from the paths specified in the 
			m_PaletteTextureID = Texture::Create(paletteJson["texturePath"]);
			m_NormalMapTextureID = Texture::Create(paletteJson["normalsPath"]);

			// the default tile size should be specified in the palette too
			m_TileSize = sf::Vector2u(paletteJson["tileSizeX"], paletteJson["tileSizeY"]);

			// then an array of tiles should exist
			for (auto& element : paletteJson["tiles"])
			{
				// place the tile data into the tile atlas

				// get an id for the tile
				TileID newID = GetNextTileID();

				// pos is the texture coordinates of the tile:
				// its location in the tile palette texture
				sf::Vector2u pos{ element["x"], element["y"] };

				// check if the tile has a size that isn't the default
				sf::Vector2u size = m_TileSize;
				if (element.find("w") != element.end())
					size.x = element["w"];
				if (element.find("h") != element.end())
					size.y = element["h"];
				
				// same process with the collider size and offest
				sf::Vector2u colliderSize = size;
				sf::Vector2i colliderOffset;
				if (element.find("collider") != element.end())
				{
					colliderOffset.x = element["collider"]["x"];
					colliderOffset.y = element["collider"]["y"];

					colliderSize.x = element["collider"]["w"];
					colliderSize.y = element["collider"]["h"];
				}

				// finally insert the tile into the atlas
				m_TileAtlas.insert(std::make_pair(newID,
					TileData{ element["name"], pos, size, colliderOffset, colliderSize }
				));
			}
		}
		catch (const std::exception& e)
		{
			// let the user know what went wrong when trying to retrieve data from the data file
			LOG_CORE_ERROR("Exception occurred: {0}", e.what());
			SFMLE_CORE_ASSERT(0, "Failed to construct palette, invalid palette JSON.");
		}

		// set the placing point for any future tiles
		m_PlaceX = ResourceManager::GetResourceHandle<sf::Texture>(m_PaletteTextureID)->getSize().x;
		m_PlaceY = 0;
	}


	TilePalette::~TilePalette()
	{
		// destroy the textures
		Texture::Destroy(m_PaletteTextureID);
		Texture::Destroy(m_NormalMapTextureID);

		// get rid of all the tile data
		m_TileAtlas.clear();
	}

	TileID TilePalette::CreateTile(const std::string& name, ResourceID tileTextureID, ResourceID tileNormalsID)
	{
		// make sure we dont have a tile with the same name already
		SFMLE_CORE_ASSERT(!TileExists(name), "A tile with that name already exists!");

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
		m_TileAtlas.insert(std::make_pair(newID, TileData{ name, sf::Vector2u(m_PlaceX, m_PlaceY), m_TileSize }));


		// work out the place to place the next tile
		// the goal of this is to keep the texture a square

		m_MaxPlaceX = std::max(m_MaxPlaceX, m_PlaceX);
		m_MaxPlaceY = std::max(m_MaxPlaceY, m_PlaceY);

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


	// simple getters for various tile information,
	// with assertions to make sure we dont retrieve data on non-existant tiles
	const sf::Vector2u& TilePalette::GetTileSize(TileID tile)
	{
		SFMLE_CORE_ASSERT(m_TileAtlas.find(tile) != m_TileAtlas.end(), "Tile has not been registered!");
		return m_TileAtlas[tile].TileSize;
	}

	const TileData& TilePalette::GetTileData(TileID tile)
	{
		SFMLE_CORE_ASSERT(m_TileAtlas.find(tile) != m_TileAtlas.end(), "Tile has not been registered!");
		return m_TileAtlas[tile];
	}
	
	const sf::Vector2u& TilePalette::GetTexCoords(TileID tile)
	{
		SFMLE_CORE_ASSERT(m_TileAtlas.find(tile) != m_TileAtlas.end(), "Tile has not been registered!");
		return m_TileAtlas[tile].TexCoords;
	}

	const sf::Vector2u& TilePalette::GetColliderSize(TileID tile)
	{
		SFMLE_CORE_ASSERT(m_TileAtlas.find(tile) != m_TileAtlas.end(), "Tile has not been registered!");
		return m_TileAtlas[tile].ColliderSize;
	}

	const sf::Vector2i& TilePalette::GetColliderOffset(TileID tile)
	{
		SFMLE_CORE_ASSERT(m_TileAtlas.find(tile) != m_TileAtlas.end(), "Tile has not been registered!");
		return m_TileAtlas[tile].ColliderOffset;
	}

	
	TileID TilePalette::GetNextTileID()
	{
		// retrieves an ID to assign to the tile
		// the value does not matter, so long as it is unique

		SFMLE_CORE_ASSERT(m_TileCount < MAX_TILES, "Too many tiles in this tilemap!");

		TileID nextID = m_AvailableTileIDs.front();
		m_AvailableTileIDs.pop();
		++m_TileCount;

		return nextID;
	}

	TileID TilePalette::GetTileByName(const std::string& name)
	{
		// retrieves the ID of the tile with the given name
		// useful if the user defined tiles with a certain name in order to
		// retrieve them later
		for (auto const& tile : m_TileAtlas)
		{
			if (tile.second.Name == name) return tile.first;
		}
		LOG_CORE_ERROR("Tile '{0}' does not exist.", name);
		SFMLE_CORE_ASSERT(0, "Could not find tile.");
		return NULL_TILE_ID;
	}

	bool TilePalette::TileExists(const std::string& name)
	{
		// check if the tile exists in the tile map
		for (auto const& tile : m_TileAtlas)
		{
			if (tile.second.Name == name) return true;
		}
		return false;
	}

	std::vector<TileID> TilePalette::GetAllTiles()
	{
		// gets the ID of all tiles in the palette

		std::vector<TileID> tiles;
		tiles.reserve(m_TileAtlas.size());
		for (auto const& tile : m_TileAtlas)
		{
			tiles.push_back(tile.first);
		}

		// after termination of this function,
		// the tile palette will not need to use this vector
		// so it can be returned with move semantics
		return std::move(tiles);
	}




	ResourceID TilePalette::Create(const sf::Vector2u& tileSize)
	{
		// creates a new tile palette object,
		// gives it to the resource manager to manage the memory
		// and then caches the palette

		// tile palettes not loaded from files cannot be shared

		TilePalette* newPalette = new TilePalette(tileSize);
		ResourceID newID = ResourceManager::ManageResource(newPalette);

		s_PaletteCache.push_back(TilePaletteCacheEntry{ newID, "", false });
		return newID;
	}

	ResourceID TilePalette::LoadFromFile(const std::string& texturePath, const std::string& normalsPath, const sf::Vector2u& tileSize, bool shared)
	{
		// check the cache to see if this palette has been loaded before
		ResourceID cached = PaletteCached(texturePath);
		// if it has then return that palette
		if (cached != NULL_RESOURCE_ID) return cached;


		// otherwise create a new palette and give it to the resource manager for managing
		TilePalette* newPalette = new TilePalette(texturePath, normalsPath, tileSize);
		ResourceID newID = ResourceManager::ManageResource(newPalette);

		// and cache it so it can be reused
		s_PaletteCache.push_back(TilePaletteCacheEntry{ newID, texturePath, shared });
		return newID;
	}

	ResourceID TilePalette::LoadFromFile(const std::string& jsonPath, bool shared)
	{
		// identical process to the above function, 
		// it only uses a different ctor for the tile palette

		ResourceID cached = PaletteCached(jsonPath);
		if (cached != NULL_RESOURCE_ID) return cached;


		TilePalette* newPalette = new TilePalette(jsonPath);
		ResourceID newID = ResourceManager::ManageResource(newPalette);

		s_PaletteCache.push_back(TilePaletteCacheEntry{ newID, jsonPath, shared });
		return newID;
	}

	void TilePalette::Destroy(ResourceID palette)
	{
		// find the palette in the cache and delete it from memory
		int index = 0;
		for (auto& p : s_PaletteCache)
		{
			if (p.PaletteID == palette)
			{
				ResourceManager::DeleteResource<TilePalette>(palette);
				s_PaletteCache.erase(s_PaletteCache.begin() + index);
				break;
			}
			++index;
		}

		// give a warning if the palette did not exist
		LOG_CORE_WARN("Attempted to delete palette '{0}' but it did not exist.", palette);
	}

	void TilePalette::DestroyAllCached()
	{
		// simple deletes every tile palette in the cache
		// this is executed at shutdown in case the user did not delete their tile palettes manually
		for (auto& p : s_PaletteCache)
		{
			ResourceManager::DeleteResource<TilePalette>(p.PaletteID);
		}
		s_PaletteCache.clear();
	}

	ResourceID TilePalette::PaletteCached(const std::string& path)
	{
		// checks if a palette from that filepath has been loaded before
		// if it does, it gives its ID
		// if it doesn't, it gives a null id
		for (auto const& palette : s_PaletteCache)
		{
			if (palette.Filepath == path && palette.Shared)
				return palette.PaletteID;
		}
		return NULL_RESOURCE_ID;
	}

}