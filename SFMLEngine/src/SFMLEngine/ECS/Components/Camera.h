#pragma once


#include <SFML/Graphics.hpp>

#include "../System.h"


namespace SFMLEngine {

	struct Camera
	{
		friend class System;

		sf::Vector2f Size;
		sf::FloatRect Viewport;
		float Zoom;
		bool Main;

		Camera();
		Camera(const sf::Vector2f& size, const sf::FloatRect& viewportSize, float zoom, bool main)
			: Size(size), Viewport(viewportSize), Zoom(zoom), Main(main)
		{}

		void SetMain(bool main = true)
		{
			Main = main;
			m_Modified = true;
		}

	private:
		bool m_Modified = true;
	};

}
