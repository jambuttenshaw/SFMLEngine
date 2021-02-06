#pragma once


#include <SFML/Graphics.hpp>

#include "../System.h"


namespace SFMLEngine {

	struct Camera
	{
		friend class System;

		sf::Vector2f Size;
		sf::Vector2f ViewportSize;

		Camera()
			: Size(), ViewportSize()
		{}
		Camera(const sf::Vector2f& size, const sf::Vector2f& viewportSize)
			: Size(size), ViewportSize(viewportSize)
		{}

	private:
		bool m_Modified = true;
	};

}
