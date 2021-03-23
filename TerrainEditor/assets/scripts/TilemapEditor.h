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
		if (!m_Active) return;


		if (Input::IsMouseButtonDown(sf::Mouse::Left))
		{
			m_Tilemap->PlaceTile(m_Tilemap->WorldToTileCoordinates(Input::GetMouseWorldPos()), m_Palette->GetAllTiles()[m_CurrentTile], true);
		}


		if (Input::IsMouseButtonDown(sf::Mouse::Right))
		{
			m_Tilemap->RemoveTile(m_Tilemap->WorldToTileCoordinates(Input::GetMouseWorldPos()));
		}


		if (!Input::IsKeyDown(sf::Keyboard::LControl))
		{
			auto wheelDelta = Input::GetMouseWheelDelta();
			if (abs(wheelDelta) > 0)
			{
				m_CurrentTile += wheelDelta;

				if (m_CurrentTile > m_Palette->GetTileCount() - 1) m_CurrentTile = 0;

				m_TilePreview->PlaceTile({ 0, 0 }, m_Palette->GetAllTiles()[m_CurrentTile], true);
			}
		}

		if (Input::IsKeyPressed(sf::Keyboard::Space))
		{
			
			m_Tilemap->Export(m_ExportPath);
			LOG_INFO("Terrain exported.");
			
		}
	}

	
	void SetTilePreviewEntity(Entity entity)
	{
		m_TilePreview = &GetComponent<Tilemap>(entity);
	}
	void SetExportPath(const std::string& path)
	{
		m_ExportPath = path;
	}


	void ActivateLayer() { m_Active = true; }
	void DectivateLayer() { m_Active = false; }


private:
	Tilemap* m_Tilemap = nullptr;
	TilePalette* m_Palette = nullptr;

	Tilemap* m_TilePreview = nullptr;
	std::string m_ExportPath;

	size_t m_CurrentTile = 0;

	bool m_Active = true;

};
