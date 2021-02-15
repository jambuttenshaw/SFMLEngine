#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/System.h"


namespace SFMLEngine {

	struct DirectionalLight
	{
		friend class System;

		sf::Vector3f Direction;
		float Intensity;
		sf::Color Color;
		bool Static;

		DirectionalLight()
			: Direction(sf::Vector3f(0, 0, 1)), Intensity(1), Color(sf::Color::White), Static(false)
		{}
		DirectionalLight(const sf::Vector3f& direction, float intensity, const sf::Color& color, bool staticLight = false)
			: Direction(direction), Intensity(intensity), Color(color), Static(staticLight)
		{}

	private:
		bool m_Modified = true;

	};

}

