#pragma once

#include <SFML/Graphics.hpp>

#include "../Constants.h"
#include "../ResourceManagement/ResourceManager.h"

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
		static ResourceID Create(const std::string& path, bool shared = true)
		{
			ResourceID newID = NULL_RESOURCE_ID;
			if (shared)
			{
				// check to see if there this texture has been loaded before
				newID = TextureCached(path);
				if (newID != NULL_RESOURCE_ID)
					return newID;
			}

			// load from file
			newID = ResourceManager::LoadFromFile<sf::Texture>(path);

			// register texture into the cache
			s_TextureCache.push_back(TextureCacheEntry{ path, newID, shared });

			return newID;
		}

		static void DestroyAllCached()
		{
			for (auto& texture : s_TextureCache)
			{
				ResourceManager::DeleteResource<sf::Texture>(texture.TextureID);
			}
			s_TextureCache.clear();
		}

	private:

		static ResourceID TextureCached(const std::string& path)
		{
			for (auto const& texture : s_TextureCache)
			{
				if (texture.Filepath == path && texture.Shared)
					return texture.TextureID;
			}
			return NULL_RESOURCE_ID;
		}

	private:

		static std::vector<TextureCacheEntry> s_TextureCache;

	};

}
