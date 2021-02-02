#pragma once

#include <GL/glew.h>
#include <SFML/Graphics.hpp>

#include "../Constants.h"
#include "../Log.h"
#include "../ResourceManagement/ResourceManager.h"

#include <unordered_map>


namespace SFMLEngine {

	struct ShaderData
	{
		ResourceID ShaderID;
		bool Lit;
	};

	class ShaderLibrary
	{
	public:

		static void Init()
		{
			// load in the shaders
			LoadNewShader("Basic", "assets/shaders/Basic.vert", "assets/shaders/Basic.frag");
			LoadNewShader("TextureMask", "assets/shaders/Basic.vert", "assets/shaders/TextureMask.frag");
			LoadNewShader("Lit", "assets/shaders/Lit.vert", "assets/shaders/Lit.frag", true);
		}

		static void Shutdown()
		{
			for (auto const& shader : s_ShaderLibrary)
			{
				ResourceManager::DeleteResource<sf::Shader>(shader.second.ShaderID);
			}
			s_ShaderLibrary.clear();
		}

		static ResourceID LoadNewShader(const std::string& name, const std::string& vertShader, const std::string& fragShader, bool lit = false)
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

			s_ShaderLibrary.insert(std::make_pair(name, ShaderData{ newShader, lit }));

			return newShader;
		}

		static ShaderData GetShaderData(const std::string& name)
		{
			auto location = s_ShaderLibrary.find(name);

			if (location == s_ShaderLibrary.end())
			{
				LOG_CORE_ERROR("Shader '{0}' does not exist.", name);
				return ShaderData{ NULL_RESOURCE_ID, false };
			}

			return s_ShaderLibrary[name];
		}

		static ResourceID GetShaderResourceID(const std::string& name)
		{
			return GetShaderData(name).ShaderID;
		}
		static bool IsLitShader(const std::string& name)
		{
			return GetShaderData(name).Lit;
		}

	private:

		static std::unordered_map<std::string, ShaderData> s_ShaderLibrary;

	};

}
