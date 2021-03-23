#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

#include "EditableTerrain.h"

class EditorManager : public ScriptableEntity
{
public:
	void Start() override
	{
		if (m_TerrainLayers->size())
		{
			SelectLayer();
		}
	}

	void Update(Timestep ts) override
	{
		if (!m_TerrainLayers->size()) return;

		// use a temporary int instead of directly using current layer
		// because m_CurrentLayer is an unsigned type
		// and it is possible for the temp to be less than 0 after a keypress
		int nextLayer = static_cast<int>(m_CurrentLayer);
		if (Input::IsKeyPressed(sf::Keyboard::Up))
		{
			nextLayer += 1;
			if (nextLayer >= m_TerrainLayers->size())
				nextLayer = 0;
		}
		if (Input::IsKeyPressed(sf::Keyboard::Down))
		{
			nextLayer -= 1;
			if (nextLayer < 0)
				nextLayer = static_cast<int>(m_TerrainLayers->size()) - 1;
		}

		if (nextLayer != m_CurrentLayer)
		{
			// switch layers
			m_CurrentLayer = static_cast<size_t>(nextLayer);
			SelectLayer();
		}

		DEBUG_DISPLAY("Current Layer", static_cast<int>(m_CurrentLayer));
	}


	void SetTerrainLayers(std::vector<EditableTerrain*>* terrainLayers)
	{
		m_TerrainLayers = terrainLayers;
	}


	void SelectLayer()
	{
		// set the current layer active and all the others inactive
		for (size_t i = 0; i < m_TerrainLayers->size(); i++)
		{
			auto& currentLayer = m_TerrainLayers->at(i);
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

private:
	// the vector of terrain layers is owned by the scene
	// but the manager has a pointer to it to control the layers
	std::vector<EditableTerrain*>* m_TerrainLayers;

	size_t m_CurrentLayer = 0;
};
