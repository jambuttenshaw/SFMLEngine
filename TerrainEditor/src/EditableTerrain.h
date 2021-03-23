#pragma once


#include <SFMLEngine.h>
#include "TilemapEditor.h"

using namespace SFMLEngine;


class EditableTerrain
{
public:
	EditableTerrain(Scene* scene, Entity tilePreviewEntity, ResourceID tilePaletteID, const std::string& terrainPath, ResourceID opaque, ResourceID translucent)
		: m_TerrainPath(terrainPath), m_TilePreviewEntity(tilePreviewEntity)
	{
		m_Scene = scene;

		m_OpaqueMat = opaque;
		m_TranslucentMat = translucent;

		m_EntityHandle = m_Scene->CreateEntity();
		m_Scene->AddComponent(m_EntityHandle, Transform{ });

		m_Scene->AddComponent(m_EntityHandle, Tilemap{ tilePaletteID, m_TerrainPath });

		// add a tilemap renderer
		m_Scene->AddComponent(m_EntityHandle, TilemapRenderer{ m_OpaqueMat, 0 });

		// add a script to control editing the terrain
		m_EditorScript = &m_Scene->AddNativeScript<TilemapEditor>(m_EntityHandle);
		m_EditorScript->SetTilePreviewEntity(m_TilePreviewEntity);
		m_EditorScript->SetExportPath(m_TerrainPath);
	}

	TilemapEditor* GetEditorScript() { return m_EditorScript; }

	void SetTranslucent(bool state)
	{
		auto& renderer = m_Scene->GetComponent<TilemapRenderer>(m_EntityHandle);
		if (state)
			renderer.ChangeMaterial(m_TranslucentMat);
		else
			renderer.ChangeMaterial(m_OpaqueMat); 
	}

private:
	// the scene this entity belongs to
	Scene* m_Scene;

	TilemapEditor* m_EditorScript = nullptr;

	Entity m_EntityHandle = INVALID_ENTITY_ID;
	std::string m_TerrainPath;

	Entity m_TilePreviewEntity = INVALID_ENTITY_ID;

	ResourceID m_OpaqueMat;
	ResourceID m_TranslucentMat;
};
