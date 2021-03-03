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
		bool mouseDown = Input::IsMouseButtonDown(sf::Mouse::Left);
		if (mouseDown && !m_LeftClick)
		{
			m_LeftClick = true;
			m_Tilemap->PlaceTile(m_Tilemap->WorldToTileCoordinates(Input::GetMouseWorldPos()), m_Palette->GetAllTiles()[m_CurrentTile], true);

		}
		else if (!mouseDown && m_LeftClick)
		{
			m_LeftClick = false;
		}



		mouseDown = Input::IsMouseButtonDown(sf::Mouse::Right);
		if (mouseDown && !m_RightClick)
		{
			m_RightClick = true;
			m_Tilemap->RemoveTile(m_Tilemap->WorldToTileCoordinates(Input::GetMouseWorldPos()));

		}
		else if (!mouseDown && m_RightClick)
		{
			m_RightClick = false;
		}



		m_CurrentTile = (m_CurrentTile + static_cast<size_t>(Input::GetMouseWheelDelta())) % m_Palette->GetTileCount();
	}

private:
	Tilemap* m_Tilemap = nullptr;
	TilePalette* m_Palette = nullptr;

	bool m_LeftClick = false;
	bool m_RightClick = false;

	size_t m_CurrentTile = 0;

};
