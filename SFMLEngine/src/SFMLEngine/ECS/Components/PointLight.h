#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/System.h"


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
			: Intensity(intensity), Range(1.0f / range), Color(color), Static(staticLight)
		{}
		
		void SetIntensity(float intensity)
		{
			Intensity = intensity;
			m_Modified = true;
		}

		void SetRange(float range)
		{
			if (range < 0) range = 0.0000001f;
			Range = 1.0f / range;

			m_Modified = true;
		}
		float GetRange()
		{
			return 1.0f / Range;
		}
		
		void SetColor(const sf::Color& color)
		{
			Color = color;
			m_Modified = true;
		}

	private:
		bool m_Modified = true;
	};

}

