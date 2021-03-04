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
		static void SetRenderSystems(std::shared_ptr<SpriteRenderSystem> sRS, std::shared_ptr<TilemapRenderSystem> tRS);
		static void Shutdown();

		static void Render();

	private:
		static std::shared_ptr<SpriteRenderSystem> s_SpriteRenderSystem;
		static std::shared_ptr<TilemapRenderSystem> s_TilemapRenderSystem;

		static sf::ContextSettings* s_ContextSettings;
	};

}
