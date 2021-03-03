#include <SFMLEngine.h>


#include "TilemapEditor.h"
#include "FollowMouse.h"

using namespace SFMLEngine;

class MainScene : public Scene
{
public:
	void Create() override
	{
		ResourceID tilePaletteID = TilePalette::LoadFromFile("assets/textures/terrain.png", "assets/textures/terrainNormals.png", sf::Vector2u(32, 32));
		TilePalette* tilePalette = ResourceManager::GetResourceHandle<TilePalette>(tilePaletteID);

		

		{
			m_Terrain = CreateEntity();
			AddComponent(m_Terrain, Transform{ });

			// create an empty tilemap
			Tilemap tilemap{ tilePaletteID };

			TileID tile0 = tilePalette->GetTileByName("Tile0");
			tilemap.PlaceTile({ 0, 0 }, tile0);

			AddComponent(m_Terrain, tilemap);

			// add a tilemap renderer
			AddComponent(m_Terrain, TilemapRenderer{ Material::Create("Basic"), 0, 0 });

			// add a script to control editing the terrain
			AddNativeScript<TilemapEditor>(m_Terrain);
		}
		

		{
			// add a camera to the scene
			m_Camera = CreateEntity();
			AddComponent(m_Camera, Transform{ });
			AddComponent(m_Camera, Camera{ });
		}


		{
			m_TilePreview = CreateEntity();
			AddComponent(m_TilePreview, Transform{ });

			// create an empty tilemap
			Tilemap tilemap{ tilePaletteID };

			TileID tile0 = tilePalette->GetTileByName("Tile0");
			tilemap.PlaceTile({ 0, 0 }, tile0);

			AddComponent(m_TilePreview, tilemap);


			// add a tilemap renderer
			ResourceID mat = Material::CreateInstance("Basic");

			Material* matPtr = ResourceManager::GetResourceHandle<Material>(mat);
			matPtr->SetProperty("u_Color", sf::Color(255, 255, 255, 127));

			AddComponent(m_TilePreview, TilemapRenderer{ mat, 0, 0 });

			AddNativeScript<FollowMouse>(m_TilePreview);
		}
	}

private:
	Entity m_Terrain;
	Entity m_TilePreview;
	Entity m_Camera;
};


class TerrainEditorApplication : public Application
{
public:
	TerrainEditorApplication()
		: Application("Terrain Editor", { 1200, 675 })
	{
		SetClearColor({ 175, 209, 230 });

		LoadScene<MainScene>();
	}
};


Application* SFMLEngine::CreateApplication()
{
	return new TerrainEditorApplication;
}
