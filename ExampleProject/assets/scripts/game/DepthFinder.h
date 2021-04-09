#pragma once

#include <SFMLEngine.h>
using namespace SFMLEngine;


class DepthFinder : public ScriptableEntity
{
public:
	void OnSceneLoaded() override
	{
		m_GroundTilemap = &GetComponent<Tilemap>(GetEntitiesWithTag("Ground")[0]);

		// remember y-axis is flipped
		m_LowestPoint = std::numeric_limits<float>::min();
		m_HighestPoint = std::numeric_limits<float>::max();
		for (auto const& tile : m_GroundTilemap->Tiles)
		{
			m_LowestPoint = std::max(m_GroundTilemap->TileSize.y * tile.Position.y, m_LowestPoint);
			m_HighestPoint = std::min(m_GroundTilemap->TileSize.y * tile.Position.y, m_HighestPoint);
		}
	}

	float GetHighestPoint()
	{
		return m_HighestPoint;
	}

	float GetLowestPoint()
	{
		return m_LowestPoint;
	}


private:
	Tilemap* m_GroundTilemap = nullptr;

	float m_HighestPoint = 0;
	float m_LowestPoint = 0;
};

