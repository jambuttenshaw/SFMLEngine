#include <SFMLEngine.h>


#include "TilemapEditor.h"

using namespace SFMLEngine;

class MainScene : public Scene
{
public:
	void Create() override
	{

		m_Terrain = CreateEntity();
		AddComponent(m_Terrain, Transform{ });


		ResourceID tilePaletteID = TilePalette::LoadFromFile("assets/textures/terrain.png", "assets/textures/terrainNormals.png", sf::Vector2u(32, 32));
		TilePalette* tilePalette = ResourceManager::GetResourceHandle<TilePalette>(tilePaletteID);

		// create an empty tilemap
		AddComponent(m_Terrain, Tilemap{ tilePaletteID });

		// add a tilemap renderer
		AddComponent(m_Terrain, TilemapRenderer{ Material::Create("Basic"), 1, 0 });

		// add a script to control editing the terrain
		AddNativeScript<TilemapEditor>(m_Terrain);

		

		// add a camera to the scene
		m_Camera = CreateEntity();
		AddComponent(m_Camera, Transform{ });
		AddComponent(m_Camera, Camera{ });
	}

private:
	Entity m_Terrain;
	Entity m_Camera;
};


class TerrainEditorApplication : public Application
{
public:
	TerrainEditorApplication()
		: Application("Terrain Editor", { 1200, 675 })
	{
		LoadScene<MainScene>();
	}
};


Application* SFMLEngine::CreateApplication()
{
	return new TerrainEditorApplication;
}
