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
		static float CalculateOrderInLayerFactor(float max);
		static float CalculateRenderLayerFactor(float max);

	private:
		static sf::ContextSettings* s_ContextSettings;
	};

}
