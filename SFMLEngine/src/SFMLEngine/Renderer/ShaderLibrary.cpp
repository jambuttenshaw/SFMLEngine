#include "ShaderLibrary.h"

namespace SFMLEngine {

	std::unordered_map<std::string, ShaderData> ShaderLibrary::s_ShaderLibrary;

	ResourceID ShaderLibrary::LoadNewShader(const std::string& name, const std::string& vertShader, const std::string& fragShader, bool lit)
	{
		ZoneScoped;

		if (s_ShaderLibrary.find(name) != s_ShaderLibrary.end())
		{
			LOG_CORE_WARN("Shader '{0}' has already been loaded.", name);
			return GetShaderResourceID(name);
		}

		ResourceID newShader = ResourceManager::LoadFromFile<sf::Shader>(vertShader, fragShader);
		if (newShader == NULL_RESOURCE_ID)
		{
			LOG_CORE_ERROR("Shader '{0}' could not be loaded.", name);
			return NULL_RESOURCE_ID;
		}

		s_ShaderLibrary.insert(std::make_pair(name, ShaderData{ newShader, lit }));

		return newShader;
	}

	ShaderData ShaderLibrary::GetShaderData(const std::string& name)
	{
		ZoneScoped;

		auto location = s_ShaderLibrary.find(name);

		if (location == s_ShaderLibrary.end())
		{
			LOG_CORE_ERROR("Shader '{0}' does not exist.", name);
			return ShaderData{ NULL_RESOURCE_ID, false };
		}

		return s_ShaderLibrary[name];
	}

}
