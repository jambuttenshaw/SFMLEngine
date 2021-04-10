#pragma once


#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/System.h"


namespace SFMLEngine {

	class Camera
	{
	public:
		friend class System;

		Camera();
		Camera(const sf::Vector2f& size, const sf::FloatRect& viewportSize, float zoom, bool main)
			: m_Size(size), m_Viewport(viewportSize), m_Zoom(zoom), m_Main(main)
		{}

		inline const sf::Vector2f& GetSize() const { return m_Size; }
		inline void SetSize(const sf::Vector2f& size) { m_Size = size; }
		
		inline const sf::FloatRect& GetViewport() const { return m_Viewport; }
		inline void SetViewport(const sf::FloatRect& viewport) { m_Viewport = viewport; }

		inline float GetZoom() const { return m_Zoom; }
		inline void SetZoom(float zoom) { m_Zoom = std::max(zoom, 0.0f); }

		inline bool IsMain() const { return m_Main; }
		inline void SetMain(bool main = true)
		{
			m_Main = main;
			m_Modified = true;
		}

	private:
		bool m_Modified = true;

		sf::Vector2f m_Size;
		sf::FloatRect m_Viewport;
		float m_Zoom;
		bool m_Main;
	};

}
