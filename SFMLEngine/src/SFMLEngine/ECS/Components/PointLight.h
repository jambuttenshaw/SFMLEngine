#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/System.h"


namespace SFMLEngine {

	class PointLight
	{
	public:
		friend class System;


		PointLight()
			: m_Range(10.0f), m_Intensity(1.0f), m_Color(sf::Color::White), m_Static(false)
		{}
		PointLight(float intensity, float range, const sf::Color& color, bool staticLight = false)
			: m_Intensity(intensity), m_Range(1.0f / range), m_Color(color), m_Static(staticLight)
		{}
		
		inline float GetIntensity() const { return m_Intensity; }
		inline void SetIntensity(float intensity)
		{
			m_Intensity = intensity;
			m_Modified = true;
		}

		inline float GetRange() const { return 1.0f / m_Range; }
		inline float GetInverseRange() const { return m_Range; }
		inline void SetRange(float range)
		{
			if (range <= 0) range = 0.0000001f;
			m_Range = 1.0f / range;

			m_Modified = true;
		}
		
		inline const sf::Color& GetColor() const { return m_Color; }
		inline void SetColor(const sf::Color& color)
		{
			m_Color = color;
			m_Modified = true;
		}

		inline bool IsStatic() const { return m_Static; }

	private:
		bool m_Modified = true;

		float m_Intensity;
		float m_Range;
		sf::Color m_Color;
		bool m_Static;
	};

}

