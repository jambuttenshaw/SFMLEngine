#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ResourceManagement/ResourceManager.h"


namespace SFMLEngine {

	class DebugTools {
	public:
		static void Init(sf::RenderWindow* window);
		static void Shutdown();

		static void Clear();

		static void DrawRect(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color);
		static void DrawAll();

	private:
		static sf::RenderWindow* s_WindowHandle;

		static ResourceID s_DebugRectTexture;
		static ResourceID s_DebugMaterial;

		static sf::VertexArray s_Geometry;
		static size_t s_TriangleIndex;
	};

}
