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
		m_Palette = m_Tilemap->GetPalette();
	}

	void Update(float ts) override
	{
		if (!m_Active) return;


		if (Input::IsKeyPressed(sf::Keyboard::E))
		{
			sf::Vector2f worldPos{ Input::GetMouseWorldPos() };
			sf::Vector2f tilePos{ m_Tilemap->TileToWorldCoordinates(m_Tilemap->WorldToTileCoordinates(worldPos)) };
			LOG_TRACE("World Pos:  X: {0}  Y: {1}   Tile Pos:  X: {2}  Y: {3}", worldPos.x, worldPos.y, tilePos.x, tilePos.y);
		}


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
				int newTile = static_cast<int>(m_CurrentTile) + wheelDelta;

				if (newTile < 0) newTile = static_cast<int>(m_Palette->GetTileCount()) - 1;
				if (newTile > m_Palette->GetTileCount() - 1) newTile = 0;

				m_CurrentTile = static_cast<size_t>(newTile);


				m_TilePreview->PlaceTile({ 0, 0 }, m_Palette->GetAllTiles()[m_CurrentTile], true);
			}
		}

		if (Input::IsKeyPressed(sf::Keyboard::Space))
		{
			if (!Input::IsKeyDown(sf::Keyboard::LControl))
			{
				Export();
			}
		}

		if (Input::IsKeyDown(sf::Keyboard::LAlt) && !Input::IsKeyDown(sf::Keyboard::LShift) && Input::IsKeyPressed(sf::Keyboard::N))
		{
			LOG_TRACE("Adding collider.");
			Application::GetApplicationHandle()->DisplayDebugInfo(true);
			AddComponent(TilemapCollider{ TilemapCollider::OptimizationLevel::High });
			m_Collider = &GetComponent<TilemapCollider>();
		}
	}

	
	void SetTilePreviewEntity(Entity entity)
	{
		m_TilePreview = &GetComponent<Tilemap>(entity);
	}
	void SetExportPath(const std::string& dir, const std::string& filename)
	{
		m_ExportPath = dir;
		m_FileName = filename;
	}


	void ActivateLayer()
	{
		m_Active = true;
		if (m_Collider)
			m_Collider->SetDisplayDebug(true);
	}
	void DectivateLayer() {
		m_Active = false;
		if (m_Collider)
			m_Collider->SetDisplayDebug(false);
	}

	void Export() 
	{
		m_Tilemap->Export(m_ExportPath + "/" + m_FileName + ".json");
		LOG_INFO("Terrain exported for '{0}'.", m_FileName);
		
		if (m_Collider)
		{
			m_Collider->Export(m_ExportPath + "/" + m_FileName + "_collider.json");
			LOG_INFO("Collider exported for '{0}'.", m_FileName);
		}
	}


private:
	Tilemap* m_Tilemap = nullptr;
	TilemapCollider* m_Collider = nullptr;
	TilePalette* m_Palette = nullptr;

	Tilemap* m_TilePreview = nullptr;
	std::string m_ExportPath;
	std::string m_FileName;

	size_t m_CurrentTile = 0;

	bool m_Active = true;

};
