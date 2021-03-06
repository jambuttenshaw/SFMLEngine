#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/Constants.h"
#include "SFMLEngine/ResourceManagement/ResourceManager.h"

namespace SFMLEngine {

	struct TextureCacheEntry
	{
		std::string Filepath;
		ResourceID TextureID;
		bool Shared;
	};

	class Texture
	{
	public:
		static ResourceID Create(const std::string& path, bool shared = true);
		static ResourceID Create(const sf::Vector2u& dimensions);
		static ResourceID Create(ResourceID font, const std::string& text, int fontSize, const sf::Color& color);

		static ResourceID Resize(ResourceID texture, const sf::Vector2u& newSize);

		static void Destroy(ResourceID textureID);
		static void DestroyAllCached();

	private:

		static ResourceID TextureCached(const std::string& path);

	private:

		static std::vector<TextureCacheEntry> s_TextureCache;

	};

}
