#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

class TilemapEditor : public ScriptableEntity
{
public:
	void Start() override
	{
		// do something when the game starts
		m_Tilemap = &GetComponent<Tilemap>();
		m_Palette = m_Tilemap->PalettePtr;
	}

	void Update(Timestep ts) override
	{
		// do something every frame
		bool mouseDown = Input::IsMouseButtonDown(sf::Mouse::Left);
		if (mouseDown && !m_Click)
		{
			m_Click = true;
			m_Tilemap->PlaceTile(m_Tilemap->WorldToTileCoordinates(Input::GetMouseWorldPos()), m_Palette->GetAllTiles()[m_CurrentTile]);

		}
		else if (!mouseDown && m_Click)
		{
			m_Click = false;
		}

		m_CurrentTile = (m_CurrentTile + static_cast<size_t>(Input::GetMouseWheelDelta())) % m_Palette->GetTileCount();
	}

private:
	Tilemap* m_Tilemap = nullptr;
	TilePalette* m_Palette = nullptr;

	bool m_Click = false;

	size_t m_CurrentTile = 0;

};
