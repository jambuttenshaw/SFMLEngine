#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

#include <filesystem>
#include "EditableTerrain.h"

class EditorManager : public ScriptableEntity
{
public:
	void Setup(Scene* scene, Entity tilePreviewEntity, ResourceID paletteID, const std::string& levelDir, ResourceID opaqueMat, ResourceID translucentMat)
	{
		m_Scene = scene;
		m_TilePreviewEntity = tilePreviewEntity;
		m_PaletteID = paletteID;
		m_LevelDir = levelDir;
		m_OpaqueMat = opaqueMat;
		m_TranslucentMat = translucentMat;
	}


	void Start() override
	{
		// load in the terrain layers
		LOG_INFO("Loading level from '{0}'", m_LevelDir);
		for (const auto& layer : std::filesystem::directory_iterator(m_LevelDir))
		{
			std::string layerPath = layer.path().stem().string();

			// dont attempt to load colliders as tilemaps
			if (layerPath.find("collider") != std::string::npos) continue;

			LOG_TRACE("Loading layer from '{0}'", layerPath);
			m_TerrainLayers.push_back(new EditableTerrain(m_Scene, m_TilePreviewEntity, m_PaletteID, m_LevelDir, layerPath, m_OpaqueMat, m_TranslucentMat));
		}
		LOG_INFO("Loading complete.");

		if (m_TerrainLayers.size())
		{
			SelectLayer();
		}
	}

	void Update(Timestep ts) override
	{
		// creating more layers
		if (Input::IsKeyDown(sf::Keyboard::LShift) && !Input::IsKeyDown(sf::Keyboard::LAlt) && Input::IsKeyPressed(sf::Keyboard::N))
		{
			LOG_INFO("Creating new layer...");
			m_TerrainLayers.push_back(new EditableTerrain(m_Scene, m_TilePreviewEntity, m_PaletteID, m_LevelDir, "layer" + std::to_string(m_TerrainLayers.size()), m_OpaqueMat, m_TranslucentMat));

			m_CurrentLayer = m_TerrainLayers.size() - 1;
			SelectLayer();
		}


		if (Input::IsKeyDown(sf::Keyboard::LControl) && Input::IsKeyPressed(sf::Keyboard::Space))
		{
			LOG_INFO("Exporting all layers...");
			for (auto& layer : m_TerrainLayers)
			{
				layer->GetEditorScript()->Export();
			}
			LOG_INFO("All layers exported.");
		}




		if (!m_TerrainLayers.size()) return;

		// use a temporary int instead of directly using current layer
		// because m_CurrentLayer is an unsigned type
		// and it is possible for the temp to be less than 0 after a keypress
		int nextLayer = static_cast<int>(m_CurrentLayer);
		if (Input::IsKeyPressed(sf::Keyboard::Up))
		{
			nextLayer += 1;
			if (nextLayer >= m_TerrainLayers.size())
				nextLayer = 0;
		}
		if (Input::IsKeyPressed(sf::Keyboard::Down))
		{
			nextLayer -= 1;
			if (nextLayer < 0)
				nextLayer = static_cast<int>(m_TerrainLayers.size()) - 1;
		}

		if (nextLayer != m_CurrentLayer)
		{
			// switch layers
			m_CurrentLayer = static_cast<size_t>(nextLayer);
			SelectLayer();
		}

		DEBUG_DISPLAY("Current Layer", static_cast<int>(m_CurrentLayer));
	}


	void SelectLayer()
	{
		// set the current layer active and all the others inactive
		for (size_t i = 0; i < m_TerrainLayers.size(); i++)
		{
			auto& currentLayer = m_TerrainLayers[i];
			if (i == m_CurrentLayer)
			{
				currentLayer->GetEditorScript()->ActivateLayer();
				currentLayer->SetTranslucent(false);
			}
			else
			{
				currentLayer->GetEditorScript()->DectivateLayer();
				currentLayer->SetTranslucent(true);
			}
		}
	}

	~EditorManager()
	{
		for (auto& terrain : m_TerrainLayers)
			delete terrain;
		m_TerrainLayers.clear();
	}

private:
	Scene* m_Scene;

	ResourceID m_PaletteID;

	std::string m_LevelDir;
	ResourceID m_OpaqueMat;
	ResourceID m_TranslucentMat;

	Entity m_TilePreviewEntity;


	// the vector of terrain layers is owned by the scene
	// but the manager has a pointer to it to control the layers
	std::vector<EditableTerrain*> m_TerrainLayers;

	size_t m_CurrentLayer = 0;
};
