#pragma once

#include "../../ResourceManagement/ResourceManager.h"


namespace SFMLEngine {

	struct TilemapRenderer
	{
		friend class System;

		ResourceID MaterialHandle;
		int RenderLayer;
		int OrderInLayer;

		// for speed when rendering
		Material* MaterialPtr = nullptr;


		TilemapRenderer()
			: MaterialHandle(NULL_RESOURCE_ID), RenderLayer(0), OrderInLayer(0)
		{}

		TilemapRenderer(ResourceID mat, int renderLayer, int orderInLayer)
			: MaterialHandle(mat), RenderLayer(renderLayer), OrderInLayer(orderInLayer)
		{
			MaterialPtr = ResourceManager::GetResourceHandle<Material>(MaterialHandle);
		}

		void SetRenderLayer(int newRenderLayer) { RenderLayer = newRenderLayer; m_Modified = true; }

		void SetOrderInLayer(int newOrderInLayer) { OrderInLayer = newOrderInLayer; m_Modified = true; }

	private:
		bool m_Modified = true;
	};

}