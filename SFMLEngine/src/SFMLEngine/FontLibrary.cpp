#include "FontLibrary.h"

#include "ResourceManagement/ResourceManager.h"


namespace SFMLEngine {

	std::unordered_map<std::string, ResourceID> FontLibrary::s_FontLibrary;

	void FontLibrary::Init()
	{
		ZoneScoped;

		// load any default fonts here
		LoadNewFont("arial", "assets/fonts/arial.ttf");
	}

	void FontLibrary::Shutdown()
	{
		ZoneScoped; 

		for (auto const& font : s_FontLibrary)
		{
			ResourceManager::DeleteResource<sf::Font>(font.second);
		}
		s_FontLibrary.clear();
	}

	ResourceID FontLibrary::LoadNewFont(const std::string& name, const std::string& filepath)
	{
		ZoneScoped;

		if (s_FontLibrary.find(name) != s_FontLibrary.end())
		{
			LOG_CORE_WARN("Font '{0}' has already been loaded.", name);
			return GetFont(name);
		}

		ResourceID newFont = ResourceManager::LoadFromFile<sf::Font>(filepath);
		if (newFont == NULL_RESOURCE_ID)
		{
			LOG_CORE_ERROR("Font '{0}' cout not be loaded.", name);
			return newFont;
		}

		s_FontLibrary.insert(std::make_pair(name, newFont));
		return newFont;
	}

	ResourceID FontLibrary::GetFont(const std::string& name)
	{
		ZoneScoped;

		if (s_FontLibrary.find(name) == s_FontLibrary.end())
		{
			LOG_CORE_ERROR("Font '{0}' does not exist.", name);
			return NULL_RESOURCE_ID;
		}

		return s_FontLibrary[name];
	}

}