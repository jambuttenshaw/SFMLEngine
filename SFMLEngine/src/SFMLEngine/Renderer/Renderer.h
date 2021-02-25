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


	public:
		static float CalculateOrderInLayerFactor(int max);
		static float CalculateRenderLayerFactor(int max);

	private:
		static sf::ContextSettings* s_ContextSettings;
	};

}
