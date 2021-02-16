#pragma once

#include <SFML/Graphics.hpp>

namespace SFMLEngine {

	class Renderer
	{
	public:

		static const sf::ContextSettings& Init();
		static void InitGLEW();
		static void Shutdown();

		static void Clear();

		static void SetOpenGLStates();

		static void SetUniforms();

	private:
		static sf::ContextSettings* s_ContextSettings;
	};

}
