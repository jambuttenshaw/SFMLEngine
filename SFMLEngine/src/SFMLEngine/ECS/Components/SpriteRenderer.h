#pragma once

#include <SFML/Graphics.hpp>

#include "../System.h"
#include "../../ResourceManagement/ResourceManager.h"
#include "../../Renderer/Material.h"


namespace SFMLEngine {

	struct SpriteRenderer
	{
		friend class System;

		ResourceID TextureHandle;
		ResourceID MaterialHandle;
		int RenderLayer;
		int OrderInLayer;
		ResourceID NormalMapHandle;
		sf::Sprite Sprite;

		// for speed when rendering
		Material* MaterialPtr = nullptr;
		sf::Texture* NormalMapPtr = nullptr;

		SpriteRenderer()
			: TextureHandle(NULL_RESOURCE_ID), MaterialHandle(NULL_RESOURCE_ID), RenderLayer(0), OrderInLayer(0), NormalMapHandle(NULL_RESOURCE_ID), Sprite()
		{}
		SpriteRenderer(ResourceID texHandle, ResourceID material, int renderLayer, int orderInLayer)
			: TextureHandle(texHandle), MaterialHandle(material), RenderLayer(renderLayer), OrderInLayer(orderInLayer), NormalMapHandle(NULL_RESOURCE_ID), Sprite()
		{
			MaterialPtr = ResourceManager::GetResourceHandle<Material>(MaterialHandle);
		}
		SpriteRenderer(ResourceID texHandle, ResourceID material, int renderLayer, int orderInLayer, ResourceID normalMap)
			: TextureHandle(texHandle), MaterialHandle(material), RenderLayer(renderLayer), OrderInLayer(orderInLayer), NormalMapHandle(normalMap), Sprite()
		{
			MaterialPtr = ResourceManager::GetResourceHandle<Material>(MaterialHandle);
			NormalMapPtr = ResourceManager::GetResourceHandle<sf::Texture>(NormalMapHandle);
		}

		sf::Texture* GetTexture() const
		{
			return ResourceManager::GetResourceHandle<sf::Texture>(TextureHandle);
		}

		Material* GetMaterial() const
		{
			return ResourceManager::GetResourceHandle<Material>(MaterialHandle);
		}

		sf::Texture* GetNormalMap() const
		{
			return ResourceManager::GetResourceHandle<sf::Texture>(NormalMapHandle);
		}

		void SetRenderLayer(int newRenderLayer) { RenderLayer = newRenderLayer; m_Modified = true; }

		void SetOrderInLayer(int newOrderInLayer) { OrderInLayer = newOrderInLayer; m_Modified = true; }

	private:
		bool m_Modified = true;
	};

}

