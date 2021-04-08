#include "SpriteRenderer.h"

namespace SFMLEngine {

	SpriteRenderer::SpriteRenderer()
		: m_TextureHandle(NULL_RESOURCE_ID), m_MaterialHandle(NULL_RESOURCE_ID), m_RenderLayer(0), m_NormalMapHandle(NULL_RESOURCE_ID), m_Sprite(), m_Offset()
	{}
	SpriteRenderer::SpriteRenderer(ResourceID texHandle, ResourceID material, int renderLayer)
		: m_TextureHandle(texHandle), m_MaterialHandle(material), m_RenderLayer(renderLayer), m_NormalMapHandle(NULL_RESOURCE_ID), m_Sprite(), m_Offset()
	{
		m_MaterialPtr = ResourceManager::GetResourceHandle<Material>(m_MaterialHandle);

		if (m_TextureHandle != NULL_RESOURCE_ID)
		{
			m_Sprite.setTexture(*GetTexture());
		}
		else
		{
			LOG_CORE_WARN("No texture was supplied for sprite renderer!");
		}
	}
	SpriteRenderer::SpriteRenderer(ResourceID texHandle, ResourceID material, int renderLayer, ResourceID normalMap)
		: m_TextureHandle(texHandle), m_MaterialHandle(material), m_RenderLayer(renderLayer), m_NormalMapHandle(normalMap), m_Sprite(), m_Offset()
	{
		m_MaterialPtr = ResourceManager::GetResourceHandle<Material>(m_MaterialHandle);
		m_NormalMapPtr = ResourceManager::GetResourceHandle<sf::Texture>(m_NormalMapHandle);

		if (m_TextureHandle != NULL_RESOURCE_ID)
		{
			m_Sprite.setTexture(*GetTexture());
		}
		else
		{
			LOG_CORE_WARN("No texture was supplied for sprite renderer!");
		}
	}

	sf::Texture* SpriteRenderer::GetTexture() const
	{
		return ResourceManager::GetResourceHandle<sf::Texture>(m_TextureHandle);
	}
	ResourceID SpriteRenderer::GetTextureHandle() const
	{
		return m_TextureHandle;
	}

	ResourceID SpriteRenderer::GetMaterialHandle() const
	{
		return m_MaterialHandle;
	}
	Material* SpriteRenderer::GetMaterial() const
	{
		return m_MaterialPtr;
	}

	sf::Texture* SpriteRenderer::GetNormalMap() const
	{
		return m_NormalMapPtr;
	}

	void SpriteRenderer::SetMaterial(ResourceID newMat)
	{
		m_MaterialHandle = newMat;
		// the pointer to the material also needs updated when the material is changed
		m_MaterialPtr = ResourceManager::GetResourceHandle<Material>(newMat);
		m_Modified = true;
	}

	void SpriteRenderer::SetRenderLayer(int newRenderLayer) { m_RenderLayer = newRenderLayer; m_Modified = true; }

	void SpriteRenderer::SetFrame(const AnimationFrame& frame, bool flipped)
	{
		if (flipped)
		{
			m_Sprite.setTextureRect(Math::FlipRect(frame.ImageRect));
			m_Offset = frame.Offset;
		}
		else
		{
			m_Sprite.setTextureRect(frame.ImageRect);
			m_Offset.y = frame.Offset.y;
		}
	}

	void SpriteRenderer::SetColor(const sf::Color& color)
	{
		m_Color = color;
		m_Sprite.setColor(color);
	}

}

