#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/Constants.h"
#include "SFMLEngine/Renderer/TilePalette.h"

#include "Transform.h"


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

	class Tilemap
	{
	public:
		friend class System;

		

		Tilemap();
		Tilemap(ResourceID tilePalette);
		Tilemap(ResourceID tilePalette, std::initializer_list<Tile> tiles);
		Tilemap(ResourceID tilePalette, const std::string& dataFile);

		void PlaceTile(const sf::Vector2i& location, TileID tileType, bool overwrite = false);
		// returns the ID of the type of tile that was removed; null tile id if no tile was removed
		TileID RemoveTile(const sf::Vector2i& location);

		inline TileID GetTileAtLocation(const sf::Vector2i& location) { return m_Tiles[GetTileIndex(location)].TileType; }

		sf::Vector2i WorldToTileCoordinates(const sf::Vector2f& worldCoords);
		sf::Vector2f TileToWorldCoordinates(const sf::Vector2i& tileCoords);

		inline void SetTransform(Transform* transform) { m_Transform = transform; }

		bool Export(const std::string& exportPath);


		inline const sf::Vector2f& GetTileSize() const { return m_TileSize; }

		inline ResourceID GetPaletteHandle() const { return m_PaletteHandle; }
		inline TilePalette* GetPalette() const { return m_PalettePtr; }

		inline const std::vector<Tile>& GetTiles() const { return m_Tiles; }
		inline const sf::VertexArray& GetGeometry() const { return m_Geometry; }

	public:
		inline static void WarnOnLoadFailure(bool flag) { s_WarnOnLoadFailure = flag; }
	private:
		static bool s_WarnOnLoadFailure;

	private:
		
		void AddTileToGeometry(Tile& tile);

		bool TileAtLocation(const sf::Vector2i& pos);
		size_t GetTileIndex(const sf::Vector2i& pos);

	private:
		bool m_Modified = false;

		sf::Vector2f m_TileSize;

		ResourceID m_PaletteHandle;
		TilePalette* m_PalettePtr = nullptr;

		std::vector<Tile> m_Tiles;

		sf::VertexArray m_Geometry;

		size_t m_TriangleIndex = 0;
		Transform* m_Transform = nullptr;
	};

}