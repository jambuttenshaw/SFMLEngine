#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ResourceManagement/ResourceManager.h"


namespace SFMLEngine {

	class DebugTools {
	public:
		DebugTools(sf::RenderWindow* window);
		~DebugTools();

		void Clear();

		void DrawRect(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color);

		void DrawAll();

	private:
		sf::RenderWindow* m_WindowHandle = nullptr;

		ResourceID m_DebugRectTexture;
		ResourceID m_DebugMaterial;

		sf::VertexArray m_Geometry;
		size_t m_TriangleIndex = 0;
	};

}
