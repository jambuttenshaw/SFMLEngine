#include <SFMLEngine.h>
#include <SFMLEngine/EntryPoint.h> // mark this file as the entry point

#include "EditableTerrain.h"

#include "FollowMouse.h"
#include "CameraController.h"
#include "EditorManager.h"


#include <filesystem>

using namespace SFMLEngine;

class MainScene : public Scene
{
public:
	void Create() override
	{
		ResourceID tilePaletteID = TilePalette::LoadFromFile("D:/dev/SFML/SFMLEngine/ExampleProject/assets/palettes/revisedTerrainPalette.json");
		TilePalette* tilePalette = ResourceManager::GetResourceHandle<TilePalette>(tilePaletteID);

		ResourceID opaqueMat = Material::CreateInstance("Basic");


		ResourceID translucentMat = Material::CreateInstance("Basic");
		Material* matPtr = ResourceManager::GetResourceHandle<Material>(translucentMat);
		matPtr->SetProperty("u_Color", sf::Color(255, 255, 255, 63));


		{
			m_TilePreview = CreateEntity();
			AddComponent(m_TilePreview, Transform{ });

			// create an empty tilemap
			Tilemap tilemap{ tilePaletteID };

			TileID tile0 = tilePalette->GetAllTiles()[0];
			tilemap.PlaceTile({ 0, 0 }, tile0);

			AddComponent(m_TilePreview, tilemap);


			// add a tilemap renderer
			ResourceID mat = Material::CreateInstance("Basic");

			Material* matPtr = ResourceManager::GetResourceHandle<Material>(mat);
			matPtr->SetProperty("u_Color", sf::Color(255, 255, 255, 127));

			AddComponent(m_TilePreview, TilemapRenderer{ mat, 1 });

			AddNativeScript<FollowMouse>(m_TilePreview);
		}


		// load in the terrain layers
		std::string levelDir = "assets/tilemaps/level1";
		LOG_INFO("Loading level from '{0}'", levelDir);
		for (const auto& layer : std::filesystem::directory_iterator(levelDir))
		{
			std::string layerPath = layer.path().string();
			LOG_TRACE("Loading layer from '{0}'", layerPath);
			m_TerrainLayers.push_back(new EditableTerrain(this, m_TilePreview, tilePaletteID, layerPath, opaqueMat, translucentMat));
		}
		LOG_INFO("Loading complete.");
		

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


		{
			m_Manager = CreateEntity();
			m_ManagerScript = &AddNativeScript<EditorManager>(m_Manager);
			m_ManagerScript->SetTerrainLayers(&m_TerrainLayers);
		}


		// print controls
		LOG_INFO("----- Terrain Editor Controls -----");
		LOG_INFO("Place tiles:      LEFT MOUSE BUTTON");
		LOG_INFO("Remove tiles:     RIGHT MOUSE BUTTON");
		LOG_INFO("Cycle tiles:      SCROLL WHEEL");
		LOG_INFO("Pan  camera:      WASD");
		LOG_INFO("Zoom camera:      LCTRL + SCROLL WHEEL");
		LOG_INFO("Cycle layers:     UP/DOWN ARROWS");
		LOG_INFO("Export tilemaps:  SPACE");
		LOG_INFO("-----------------------------------");
	}

	~MainScene()
	{
		for (auto& terrain : m_TerrainLayers)
			delete terrain;
		m_TerrainLayers.clear();
	}

private:
	std::vector<EditableTerrain*> m_TerrainLayers;

	Entity m_Manager;
	EditorManager* m_ManagerScript = nullptr;

	Entity m_Camera;
	Entity m_TilePreview;
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
