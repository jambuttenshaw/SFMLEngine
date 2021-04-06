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
		Tilemap(ResourceID tilePalette, const std::string& dataFile);

		void PlaceTile(const sf::Vector2i& location, TileID tileType, bool overwrite = false);
		// returns the ID of the type of tile that was removed; null tile id if no tile was removed
		TileID RemoveTile(const sf::Vector2i& location);

		sf::Vector2i WorldToTileCoordinates(const sf::Vector2f& worldCoords);

		void SetTransform(Transform* transform) { m_Transform = transform; }

		bool Export(const std::string& exportPath);

	public:
		static void WarnOnLoadFailure(bool flag) { s_WarnOnLoadFailure = flag; }
	private:
		static bool s_WarnOnLoadFailure;

	private:
		
		void AddTileToGeometry(Tile& tile);

		bool TileAtLocation(const sf::Vector2i& pos);
		size_t GetTileIndex(const sf::Vector2i& pos);

	private:
		bool m_Modified = false;

		size_t m_TriangleIndex = 0;
		Transform* m_Transform = nullptr;
	};

}