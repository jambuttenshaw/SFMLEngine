#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/System.h"
#include "SFMLEngine/ResourceManagement/ResourceManager.h"
#include "SFMLEngine/Renderer/Material.h"
#include "Animable.h"
#include "SFMLEngine/Math.h"


namespace SFMLEngine {

	class SpriteRenderer : public Animable
	{
	public:

		friend class System;


		SpriteRenderer();
		SpriteRenderer(ResourceID texHandle, ResourceID material, int renderLayer);
		SpriteRenderer(ResourceID texHandle, ResourceID material, int renderLayer, ResourceID normalMap);

		sf::Texture* GetTexture() const;
		ResourceID GetTextureHandle() const;

		sf::Texture* GetNormalMap() const;

		void SetMaterial(ResourceID newMat);
		Material* GetMaterial() const;
		ResourceID GetMaterialHandle() const;

		int GetRenderLayer() { return m_RenderLayer; }
		void SetRenderLayer(int newRenderLayer);

		const sf::Sprite& GetSpriteObject() { return m_Sprite; }

		virtual void SetFrame(const AnimationFrame& frame, bool flipped) override;
		const sf::Vector2f& GetOffset() { return m_Offset; }

		void SetFlippedNormals(bool state) { m_FlipNormals = state; }
		bool GetFlippedNormals() { return m_FlipNormals; }

		const sf::Color& GetColor() { return m_Color; }
		void SetColor(const sf::Color& color);

	private:
		bool m_Modified = true;

		ResourceID m_TextureHandle;
		ResourceID m_MaterialHandle;
		ResourceID m_NormalMapHandle;

		sf::Sprite m_Sprite;

		int m_RenderLayer;

		sf::Vector2f m_Offset;
		bool m_FlipNormals = false;

		// for speed when rendering
		Material* m_MaterialPtr = nullptr;
		sf::Texture* m_NormalMapPtr = nullptr;

		sf::Color m_Color = sf::Color::White;
	};

}

