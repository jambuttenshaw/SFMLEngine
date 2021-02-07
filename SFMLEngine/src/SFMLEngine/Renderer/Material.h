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

		UniformData(T data) : Data(data) {}
	};


	struct Uniform
	{
		std::string Name;
		GLenum Type;
		void* Data;
	};

	struct MaterialData;

	class Material
	{
	public:
		Material(const std::string& shaderName);
		~Material();

		sf::Shader* SetUniforms();
		sf::Shader* GetShaderPtr() { return m_ShaderPtr; }
		bool IsLit() { return m_Lit; }

		// even though the function bodies are all the same,
		// I overloaded SetProperty for each allowed data type to be assigned to a shader uniform
		// to stop any random data type being passed into SetUniform

		// there would be no way for me to easily validate data types being passed into SetUniform since they just
		// get casted into a void*, so using these functions effectively perform validation that the correct data types are being passed in

		void SetProperty(const std::string& propertyName, float value) { SetUniform(propertyName, value); }
		void SetProperty(const std::string& propertyName, const sf::Vector2f& value) { SetUniform(propertyName, value); }
		void SetProperty(const std::string& propertyName, const sf::Vector3f& value) { SetUniform(propertyName, value); }
		void SetProperty(const std::string& propertyName, const sf::Color& value) { SetUniform(propertyName, value); }
		void SetProperty(const std::string& propertyName, int value) { SetUniform(propertyName, value); }
		void SetProperty(const std::string& propertyName, const sf::Vector2i& value) { SetUniform(propertyName, value); }
		void SetProperty(const std::string& propertyName, const sf::Vector3i& value) { SetUniform(propertyName, value); }
		void SetProperty(const std::string& propertyName, bool value) { SetUniform(propertyName, value); }
		void SetProperty(const std::string& propertyName, const sf::Transform& value) { SetUniform(propertyName, value); }
		void SetProperty(const std::string& propertyName, ResourceID value) { SetUniform(propertyName, value); }


	public:

		// create a new material using the shader
		// or if one already exists then return the resource ID of it
		static ResourceID Create(const std::string& shader);
		static ResourceID CreateInstance(const std::string& shader);

		// get all the materials in use
		static const std::vector<MaterialData>& GetAllMaterialsInUse() { return s_MaterialCache; }

		// deletes all cached materials from memory
		// should be called on shutdown
		static void DestroyAllCached();

		// set whether warnings are given out if uniform doesnt exist
		static void WarnOnUnknownUniform(bool flag) { s_WarnOnUnknownUniform = flag; }


	private:

		void Init();

		// templated function to set the values of uniforms
		// the values are held by the material class and only uploaded to the gpu prior to rendering
		template<typename T>
		void SetUniform(const std::string& uniformName, T value)
		{
			// error checking to make sure uniform exists
			if (!UniformExists(uniformName) && s_WarnOnUnknownUniform)
			{
				LOG_CORE_WARN("Uniform {0} does not exist.", uniformName);
				return;
			}

			UniformData<T>* data = new UniformData(value);

			// assign data into uniforms vector
			auto& uniform = GetUniform(uniformName);
			// cast to a void* since we dont know the data type at compile time
			uniform.Data = (void*)(data);
		}

		bool UniformExists(const std::string& name);
		Uniform& GetUniform(const std::string& name);

	private:

		static ResourceID MaterialCached(const std::string& shaderName);

	private:
		ResourceID m_ShaderResourceID;
		sf::Shader* m_ShaderPtr;
		bool m_Lit = false;

		std::vector<Uniform> m_Uniforms;

	private:
		static std::vector<MaterialData> s_MaterialCache;
		static bool s_WarnOnUnknownUniform;

	};


	struct MaterialData
	{
		std::string ShaderName;
		ResourceID MaterialID;
		bool Shared;
		bool Lit;
		// to save time retrieving resources each frame
		Material* MaterialPtr;
		sf::Shader* ShaderPtr;
	};

}
