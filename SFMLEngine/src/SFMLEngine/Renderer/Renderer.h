#pragma once

#include <SFML/Graphics.hpp>

#include "DebugTools.h"

namespace SFMLEngine {

	class Renderer
	{
	public:

		static const sf::ContextSettings& Init();
		static void InitGLEW();
		static void Shutdown();

		static void InitDebug(sf::RenderWindow* handle);

		static void Clear();

		static void SetOpenGLStates();

		static void SetUniforms();


		static void DrawDebugRect(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color);
		static void DrawDebug();


	private:
		static sf::ContextSettings* s_ContextSettings;

		static DebugTools* s_DebugTools;
	};

}
