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

		ResourceID TextureHandle;
		sf::Sprite Sprite;

		GUIImage()
			: TextureHandle(NULL_RESOURCE_ID), Sprite()
		{}
		GUIImage(ResourceID texture)
			: TextureHandle(texture), Sprite()
		{}


		virtual sf::Vector2f GetSize() override { return static_cast<sf::Vector2f>(Sprite.getTexture()->getSize()); }
		virtual void SetScale(const sf::Vector2f& scale) override { Sprite.setScale(scale); }
		virtual void SetFrame(const AnimationFrame& frame, bool flipped) override
		{ 
			Sprite.setTextureRect(frame.ImageRect);
		}

	private:
		bool m_Modified = false;
	};


}
