#pragma once

#include "SFMLEngine/ResourceManagement/ResourceManager.h"
#include "SFMLEngine/Renderer/Material.h"

namespace SFMLEngine {

	class TilemapRenderer
	{
	public:
		friend class System;


		TilemapRenderer()
			: m_MaterialHandle(NULL_RESOURCE_ID), m_RenderLayer(0)
		{}

		TilemapRenderer(ResourceID mat, int renderLayer)
			: m_MaterialHandle(mat), m_RenderLayer(renderLayer)
		{
			m_MaterialPtr = ResourceManager::GetResourceHandle<Material>(m_MaterialHandle);
		}

		inline ResourceID GetMaterialHandle() const { return m_MaterialHandle; }
		inline Material* GetMaterial() const { return m_MaterialPtr; }
		inline void ChangeMaterial(ResourceID newMat)
		{
			m_MaterialHandle = newMat;
			m_MaterialPtr = ResourceManager::GetResourceHandle<Material>(newMat);
			m_Modified = true;
		}

		inline int GetRenderLayer() const { return m_RenderLayer; }
		inline void SetRenderLayer(int newRenderLayer) { m_RenderLayer = newRenderLayer; m_Modified = true; }

	private:
		bool m_Modified = true;

		ResourceID m_MaterialHandle;
		int m_RenderLayer;

		// for speed when rendering
		Material* m_MaterialPtr = nullptr;
	};

}