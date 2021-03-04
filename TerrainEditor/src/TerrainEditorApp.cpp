#include <SFMLEngine.h>


#include "TilemapEditor.h"
#include "FollowMouse.h"
#include "CameraController.h"

using namespace SFMLEngine;

class MainScene : public Scene
{
public:
	void Create() override
	{
		ResourceID tilePaletteID = TilePalette::LoadFromFile("assets/textures/terrain.png", "assets/textures/terrain_n.png", sf::Vector2u(32, 32));
		TilePalette* tilePalette = ResourceManager::GetResourceHandle<TilePalette>(tilePaletteID);

		

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

			AddComponent(m_TilePreview, TilemapRenderer{ mat, 1 });

			AddNativeScript<FollowMouse>(m_TilePreview);
		}

		{
			m_Terrain = CreateEntity();
			AddComponent(m_Terrain, Transform{ });

			// create an empty tilemap
			AddComponent(m_Terrain, Tilemap{ tilePaletteID });

			// add a tilemap renderer
			AddComponent(m_Terrain, TilemapRenderer{ Material::Create("Basic"), 0 });

			// add a script to control editing the terrain
			auto& script = AddNativeScript<TilemapEditor>(m_Terrain);
			script.SetTilePreviewEntity(m_TilePreview);
		}
		

		{
			// add a camera to the scene
			m_Camera = CreateEntity();
			AddComponent(m_Camera, Transform{ });
			AddComponent(m_Camera, Camera{ });

			AddNativeScript<CameraController>(m_Camera);
		}


		{
			m_CentreMarker = CreateEntity();
			AddComponent(m_CentreMarker, Transform{ {-32, -32} });

			AddComponent(m_CentreMarker, SpriteRenderer{
				Texture::Create("assets/textures/centreMarker.png"),
				Material::Create("Basic"),
				1 });
		}
	}

private:
	Entity m_Terrain;
	Entity m_TilePreview;

	Entity m_Camera;

	Entity m_CentreMarker;
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
