#pragma once

#include "SFMLEngine/ECS/System.h"
#include "GUIElement.h"
#include "../Animable.h"
#include "SFMLEngine/ResourceManagement/ResourceManager.h"

namespace SFMLEngine {


	class GUIImage : public GUIElement, public Animable
	{
	public:

		friend class System;

		GUIImage()
			: m_TextureHandle(NULL_RESOURCE_ID), m_Sprite()
		{}
		GUIImage(ResourceID texture)
			: m_TextureHandle(texture), m_Sprite()
		{
			m_Sprite.setTexture(*ResourceManager::GetResourceHandle<sf::Texture>(m_TextureHandle));
		}

		inline const sf::Sprite& GetSpriteObject() const { return m_Sprite; }

		inline virtual sf::Vector2f GetSize() const override { return static_cast<sf::Vector2f>(m_Sprite.getTexture()->getSize()); }
		inline virtual void SetScale(const sf::Vector2f& scale) override { m_Sprite.setScale(scale); }
		inline virtual void SetFrame(const AnimationFrame& frame, bool flipped) override
		{ 
			m_Sprite.setTextureRect(frame.GetRect());
		}

		inline void SetColor(const sf::Color& c)
		{
			m_Color = c;
			m_Sprite.setColor(c);
		}
		inline const sf::Color& GetColor() const { return m_Color; }

	private:
		bool m_Modified = false;

		ResourceID m_TextureHandle;
		sf::Sprite m_Sprite;
		sf::Color m_Color;
	};


}
