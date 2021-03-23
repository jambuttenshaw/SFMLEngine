#pragma once

#include "SFMLEngine/ResourceManagement/ResourceManager.h"
#include "SFMLEngine/Renderer/Material.h"

namespace SFMLEngine {

	struct TilemapRenderer
	{
		friend class System;

		ResourceID MaterialHandle;
		int RenderLayer;

		// for speed when rendering
		Material* MaterialPtr = nullptr;


		TilemapRenderer()
			: MaterialHandle(NULL_RESOURCE_ID), RenderLayer(0)
		{}

		TilemapRenderer(ResourceID mat, int renderLayer)
			: MaterialHandle(mat), RenderLayer(renderLayer)
		{
			MaterialPtr = ResourceManager::GetResourceHandle<Material>(MaterialHandle);
		}

		void ChangeMaterial(ResourceID newMat)
		{
			MaterialHandle = newMat;
			MaterialPtr = ResourceManager::GetResourceHandle<Material>(newMat);
			m_Modified = true;
		}

		void SetRenderLayer(int newRenderLayer) { RenderLayer = newRenderLayer; m_Modified = true; }

	private:
		bool m_Modified = true;
	};

}