#pragma once

#include <SFML/Graphics.hpp>


namespace SFMLEngine {

	enum class GUIElementType
	{
		Invalid = 0, Image, Text
	};

	class GUIElement
	{
	public:
		virtual ~GUIElement() = default;

		virtual sf::Vector2f GetSize() = 0;
		virtual void SetScale(const sf::Vector2f& scale) = 0;
	};

}
