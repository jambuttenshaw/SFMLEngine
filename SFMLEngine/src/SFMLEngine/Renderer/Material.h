#pragma once

#include <GL/glew.h>
#include <SFML/Graphics.hpp>

#include "../Constants.h"
#include "../ResourceManagement/ResourceManager.h"
#include "ShaderLibrary.h"

#include <vector>
#include <unordered_map>


namespace SFMLEngine {

	template<typename T>
	struct UniformData
	{
		T Data;
	};


	struct Uniform
	{
		std::string Name;
		GLenum Type;
		void* Data;
	};


	class Material
	{
	public:
		Material(const std::string& shaderName);
		Material(ResourceID shaderResourceID);
		~Material();

		sf::Shader* Bind();

		template<typename T>
		void SetUniform(const std::string& uniformName, T value)
		{
			// error checking to make sure uniform exists
			if (!UniformExists(uniformName))
			{
				LOG_CORE_ERROR("Uniform {0} does not exist.", uniformName);
				SFMLE_CORE_ASSERT(0, "Could not set uniform.");
			}

			UniformData<T> data = new UniformData<T>;
			data->Data = value;

			// assign data into uniforms vector
			auto& uniform = GetUniform(uniformName);
			// cast to a void* since we dont know the data type at compile time
			uniform.Data = static_cast<void*>(data);
		}

	private:

		void Init();

		bool UniformExists(const std::string& name);
		Uniform& GetUniform(const std::string& name);

	private:
		ResourceID m_ShaderResourceID;

		std::vector<Uniform> m_Uniforms;
	};

}
