#pragma once

#include <SFML/Graphics.hpp>

#include "../System.h"


namespace SFMLEngine {

	struct PointLight
	{
		friend class System;

		float Intensity;
		float Range;
		sf::Color Color;
		bool Static;

		PointLight()
			: Range(10.0f), Intensity(1.0f), Color(sf::Color::White), Static(false)
		{}
		PointLight(float intensity, float range, const sf::Color& color, bool staticLight = false)
			: Intensity(intensity), Range(range), Color(color), Static(staticLight)
		{}

	private:
		bool m_Modified = false;
	};

}

