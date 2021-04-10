#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/System.h"
#include "SFMLEngine/Math.h"


namespace SFMLEngine {

	class DirectionalLight
	{
	public:
		friend class System;

		DirectionalLight()
			: m_Direction(sf::Vector3f(0, 0, 1)), m_Intensity(1), m_Color(sf::Color::White), m_Static(false)
		{}
		DirectionalLight(const sf::Vector3f& direction, float intensity, const sf::Color& color, bool staticLight = false)
			: m_Direction(direction), m_Intensity(intensity), m_Color(color), m_Static(staticLight)
		{}
		DirectionalLight(float xAngle, float yAngle, float intensity, const sf::Color& color, bool staticLight = false)
			: m_Intensity(intensity), m_Color(color), m_Static(staticLight)
		{
			m_Direction = { sinf(xAngle), sinf(yAngle), cosf(xAngle) * cosf(yAngle) };
		}

		inline const sf::Vector3f& GetDirection() const { return m_Direction; }
		inline void SetDirection(float xAngle, float yAngle)
		{
			m_Direction = Math::Normalize({ sinf(xAngle), sinf(yAngle), cosf(xAngle) * cosf(yAngle) });
		}

		inline float GetIntensity() const { return m_Intensity; }
		inline void SetIntensity(float intensity) { m_Intensity = intensity; }

		inline const sf::Color& GetColor() const { return m_Color; }
		inline void SetColor(const sf::Color& color) { m_Color = color; }

		inline bool IsStatic() const { return m_Static; }

	private:
		bool m_Modified = true;

		sf::Vector3f m_Direction;
		float m_Intensity;
		sf::Color m_Color;
		bool m_Static;

	};

}

