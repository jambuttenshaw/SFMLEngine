#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/System.h"
#include "SFMLEngine/ResourceManagement/ResourceManager.h"


namespace SFMLEngine {

	struct Text
	{
		friend class System;

		ResourceID Font;
		std::string String;
		int FontSize;
		sf::Color TextColor;
		sf::Text TextObject;

		Text()
			: Font(NULL_RESOURCE_ID), String(), FontSize(0), TextColor(sf::Color::White), TextObject()
		{}
		Text(ResourceID font, const std::string& textString, int fontSize, const sf::Color& textColor)
			: Font(font), String(textString), FontSize(fontSize), TextColor(textColor), TextObject()
		{}

	private:
		bool m_Modified = true;
	};

}
