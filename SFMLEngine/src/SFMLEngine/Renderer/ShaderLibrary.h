#pragma once

#include <GL/glew.h>
#include <SFML/Graphics.hpp>

#include "SFMLEngine/Constants.h"
#include "SFMLEngine/Core.h"
#include "SFMLEngine/ResourceManagement/ResourceManager.h"

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
			ZoneScoped;

			// load in the shaders
			LoadNewShader("Basic", "assets/shaders/Basic.vert", "assets/shaders/Basic.frag");
			LoadNewShader("TextureMask", "assets/shaders/Basic.vert", "assets/shaders/TextureMask.frag");
			LoadNewShader("Lit", "assets/shaders/Lit.vert", "assets/shaders/Lit.frag", true);
			LoadNewShader("Debug", "assets/shaders/Basic.vert", "assets/shaders/Debug.frag");
		}

		static void Shutdown()
		{
			ZoneScoped;

			for (auto const& shader : s_ShaderLibrary)
			{
				ResourceManager::DeleteResource<sf::Shader>(shader.second.ShaderID);
			}
			s_ShaderLibrary.clear();
		}

		static ResourceID LoadNewShader(const std::string& name, const std::string& vertShader, const std::string& fragShader, bool lit = false);

		static ShaderData GetShaderData(const std::string& name);
		

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
