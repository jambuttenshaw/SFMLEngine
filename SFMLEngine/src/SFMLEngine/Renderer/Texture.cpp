#include "Texture.h"

namespace SFMLEngine {
	std::vector<TextureCacheEntry> Texture::s_TextureCache;

	ResourceID Texture::Create(const std::string& path, bool shared)
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

	ResourceID Texture::Create(const sf::Vector2u& dimensions)
	{
		sf::Texture* newTexture = new sf::Texture;

		if (!newTexture->create(dimensions.x, dimensions.y))
		{
			// error - for some reason sfml was unable to create a texture
			delete newTexture;
			SFMLE_CORE_ASSERT(0, "Unable to create texture!");
			return NULL_RESOURCE_ID;
		}

		// submit it to the resource manager for safekeeping
		ResourceID newID = ResourceManager::ManageResource(newTexture);

		// register this texture into the cache so it can be deleted automatically safely at shutdown
		// textures not loaded from files cannot be shared as there is no way to tell
		// that the textures are identical
		s_TextureCache.push_back(TextureCacheEntry{ "", newID, false });

		return newID;
	}

	ResourceID Texture::Create(ResourceID font, const std::string& text, int fontSize, const sf::Color& color)
	{
		// create a texture containing rendered text
		// first step is to render the text
		sf::Text textObj;
		textObj.setFont(*ResourceManager::GetResourceHandle<sf::Font>(font));
		textObj.setCharacterSize(fontSize);
		textObj.setFillColor(color);
		textObj.setString(text);

		sf::FloatRect textBounds{ textObj.getGlobalBounds() };
		textObj.setPosition(0, - 5 * textBounds.height / 6);

		// then create a render texture the size of the text
		sf::RenderTexture renderTexture;
		if (!renderTexture.create(
			static_cast<unsigned int>(textBounds.width),
			static_cast<unsigned int>(textBounds.height),
			sf::ContextSettings()
		))
		{
			SFMLE_CORE_ASSERT(0, "Unable to create texture!");
			return NULL_RESOURCE_ID;
		}

		// draw the text onto the render texture
		renderTexture.draw(textObj);
		renderTexture.display();

		// then we can create a regular texture from the render texture
		sf::Texture* newTexture = new sf::Texture(renderTexture.getTexture());

		// submit it to the resource manager for safekeeping
		ResourceID newID = ResourceManager::ManageResource(newTexture);

		// register this texture into the cache so it can be deleted automatically safely at shutdown
		// textures not loaded from files cannot be shared as there is no way to tell
		// that the textures are identical
		s_TextureCache.push_back(TextureCacheEntry{ "", newID, false });

		return newID;
	}

	ResourceID Texture::Resize(ResourceID texture, const sf::Vector2u& newSize)
	{
		// to resize the image, we want to create a new image of the desired size,
		// copy the contents of the old image onto it, then destroy the old image

		// get a handle to the texture we want to resize
		sf::Texture* texHandle = ResourceManager::GetResourceHandle<sf::Texture>(texture);
		sf::Vector2u texSize = texHandle->getSize();

		// in case someone wants to resize the image to the same size they already have
		// we just hand the same texture back to them
		if (texSize == newSize) return texture;
		if (texSize.x > newSize.x && texSize.y > newSize.y)
		{
			LOG_CORE_WARN("Resizing textures to smaller size is unsupported.");
			return texture;
		}

		ResourceID newTex = Create(newSize);
		sf::Texture* newTexHandle = ResourceManager::GetResourceHandle<sf::Texture>(newTex);

		// since the new texture must be larger, we can just copy the old texture right onto the new one
		newTexHandle->update(*texHandle, 0, 0);

		// now the contents of the texture have been copied
		// we can delete the old one
		Destroy(texture);

		// return the new texture
		return newTex;
	}

	void Texture::Destroy(ResourceID textureID)
	{
		
		for (int index = 0; index < s_TextureCache.size(); index++)
		{
			if (textureID == s_TextureCache[index].TextureID)
			{
				ResourceManager::DeleteResource<sf::Texture>(textureID);
				s_TextureCache.erase(s_TextureCache.begin() + index);
				return;
			}
		}
		LOG_CORE_WARN("Texture '{0}' was attempted to be destroyed, but it did not exist.", textureID);
	}

	void Texture::DestroyAllCached()
	{
		for (auto& texture : s_TextureCache)
		{
			ResourceManager::DeleteResource<sf::Texture>(texture.TextureID);
		}
		s_TextureCache.clear();
	}

	ResourceID Texture::TextureCached(const std::string& path)
	{
		for (auto const& texture : s_TextureCache)
		{
			if (texture.Filepath == path && texture.Shared)
				return texture.TextureID;
		}
		return NULL_RESOURCE_ID;
	}

}
