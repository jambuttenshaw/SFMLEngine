#pragma once

#include "SFMLEngine/ECS/System.h"
#include "GUIElement.h"
#include "SFMLEngine/ResourceManagement/ResourceManager.h"

namespace SFMLEngine {


	class GUIImage : public GUIElement
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

	private:
		bool m_Modified = false;
	};


}
