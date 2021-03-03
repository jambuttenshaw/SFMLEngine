#pragma once

#include <SFML/Graphics.hpp>

namespace SFMLEngine {

	class SpriteRenderSystem;
	class TilemapRenderSystem;

	class Renderer
	{
	public:

		static const sf::ContextSettings& Init();
		static void InitGLEW();
		static void SetRenderSystems(std::shared_ptr<SpriteRenderSystem>, std::shared_ptr<TilemapRenderSystem>);
		static void Shutdown();

		static void Clear();

		static void Render();


	public:
		static float CalculateOrderInLayerFactor(int max);
		static float CalculateRenderLayerFactor(int max);

		static std::shared_ptr<SpriteRenderSystem> s_SpriteRenderSystem;
		static std::shared_ptr<TilemapRenderSystem> s_TilemapRenderSystem;

	private:
		static sf::ContextSettings* s_ContextSettings;
	};

}
