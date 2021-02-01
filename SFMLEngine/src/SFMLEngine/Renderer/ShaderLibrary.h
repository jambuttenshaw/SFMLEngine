#pragma once

#include <GL/glew.h>
#include <SFML/Graphics.hpp>

#include "../Constants.h"
#include "../Log.h"
#include "../ResourceManagement/ResourceManager.h"

#include <unordered_map>


namespace SFMLEngine {

	class ShaderLibrary
	{
	public:

		static void Init()
		{
			// load in the shaders
			LoadNewShader("Basic", "assets/shaders/Basic.vert", "assets/shaders/Basic.frag");
			LoadNewShader("TextureMask", "assets/shaders/Basic.vert", "assets/shaders/TextureMask.frag");
		}

		static void Shutdown()
		{
			for (auto const& shader : s_ShaderLibrary)
			{
				ResourceManager::DeleteResource<sf::Shader>(shader.second);
			}
			s_ShaderLibrary.clear();
		}

		static ResourceID LoadNewShader(const std::string& name, const std::string& vertShader, const std::string& fragShader)
		{
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

			s_ShaderLibrary.insert(std::make_pair(name, newShader));

			return newShader;
		}

		static ResourceID GetShaderResourceID(const std::string& name)
		{
			auto location = s_ShaderLibrary.find(name);

			if (location == s_ShaderLibrary.end())
			{
				LOG_CORE_ERROR("Shader '{0}' does not exist.", name);
				return NULL_RESOURCE_ID;
			}

			return s_ShaderLibrary[name];
		}

	private:

		static std::unordered_map<std::string, ResourceID> s_ShaderLibrary;

	};

}
