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

			{
				sf::Shader* shader = ResourceManager::GetResourceHandle<sf::Shader>(newShader);
				GLuint nativeShaderHandle = shader->getNativeHandle();

				GLint i;
				GLint count;

				GLint size; // size of the variable
				GLenum type; // type of the variable (float, vec3 or mat4, etc)

				const GLsizei bufSize = 16; // maximum name length
				GLchar varName[bufSize]; // variable name in GLSL
				GLsizei length; // name length

				glGetProgramiv(nativeShaderHandle, GL_ACTIVE_UNIFORMS, &count);
				// LOG_CORE_TRACE("Active uniforms: {0}", count);
				// printf("Active Uniforms: %d\n", count);

				for (i = 0; i < count; i++)
				{
					glGetActiveUniform(nativeShaderHandle, (GLuint)i, bufSize, &length, &size, &type, varName);
					// LOG_CORE_TRACE("Uniform #{0} Type: {1} Name: {2}", i, type, varName);
					// printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
				}
			}

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
