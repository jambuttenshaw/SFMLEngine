#include "Material.h"


namespace SFMLEngine {

	std::unordered_map<std::string, ResourceID> Material::s_Materials;

	Material::Material(const std::string& shaderName)
	{
		m_ShaderResourceID = ShaderLibrary::GetShaderResourceID(shaderName);
		Init();
	}

	Material::Material(ResourceID shaderResourceID)
		: m_ShaderResourceID(shaderResourceID)
	{
		Init();
	}

	void Material::Init()
	{
		// we need to retrieve all of the uniforms from the shader
		// all uniforms with the 'u_' prefix should be exposed to the material

		// get the native opengl handle of the shader object
		GLuint nativeShaderHandle = ResourceManager::GetResourceHandle<sf::Shader>(m_ShaderResourceID)->getNativeHandle();

		// the number of uniforms used in the shader
		GLint uniformCount;

		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		const GLsizei bufferSize = 32; // maximum name length
		GLchar uniformName[bufferSize]; // variable name in GLSL
		GLsizei length; // name length

		// get the number of uniforms used in the shader
		glGetProgramiv(nativeShaderHandle, GL_ACTIVE_UNIFORMS, &uniformCount);

		for (GLuint i = 0; i < (GLuint)uniformCount; i++)
		{
			// get the information about the current uniform
			glGetActiveUniform(nativeShaderHandle, i, bufferSize, &length, &size, &type, uniformName);
			// LOG_CORE_TRACE("Uniform #{0} Type: {1} Name: {2}", i, type, varName);

			// we only want to deal with the uniform if its name begins with 'u_'
			// the length of the name must therefore be greater than 2
			if (length > 2)
			{
				if (uniformName[0] == 'u' && uniformName[1] == '_')
				{
					// the uniform should be exposed to the material

					// create a new uniform struct to hold the data about it
					Uniform newUniform;
					// its easier to deal with std::string rather than char* so we do a quick convert here
					newUniform.Name = std::string(uniformName);
					// store the type of the uniform
					// this is used for retrieving the data later
					newUniform.Type = type;
					// a pointer to the data the uniform contains
					// this is void* since we dont know what data type it will be in advance
					// but we can use the Type attribute to retrieve it to the correct data type
					newUniform.Data = nullptr;

					m_Uniforms.push_back(newUniform);
				}
			}
		}
	}

	Material::~Material()
	{
		// delete all of the shader data
		for (Uniform& uniform : m_Uniforms)
		{
			switch (uniform.Type)
			{
			case GL_FLOAT:				delete static_cast<UniformData<float>*>(uniform.Data); break;
			case GL_FLOAT_VEC2:			delete static_cast<UniformData<sf::Glsl::Vec2>*>(uniform.Data); break;
			case GL_FLOAT_VEC3:			delete static_cast<UniformData<sf::Glsl::Vec3>*>(uniform.Data); break;
			case GL_FLOAT_VEC4:			delete static_cast<UniformData<sf::Glsl::Vec4>*>(uniform.Data); break;
			case GL_INT:				delete static_cast<UniformData<int>*>(uniform.Data); break;
			case GL_INT_VEC2:			delete static_cast<UniformData<sf::Glsl::Ivec2>*>(uniform.Data); break;
			case GL_INT_VEC3:			delete static_cast<UniformData<sf::Glsl::Ivec3>*>(uniform.Data); break;
			case GL_INT_VEC4:			delete static_cast<UniformData<sf::Glsl::Ivec4>*>(uniform.Data); break;
			case GL_BOOL:				delete static_cast<UniformData<bool>*>(uniform.Data); break;
			case GL_BOOL_VEC2:			delete static_cast<UniformData<sf::Glsl::Bvec2>*>(uniform.Data); break;
			case GL_BOOL_VEC3:			delete static_cast<UniformData<sf::Glsl::Bvec3>*>(uniform.Data); break;
			case GL_BOOL_VEC4:			delete static_cast<UniformData<sf::Glsl::Bvec4>*>(uniform.Data); break;
			case GL_FLOAT_MAT3:			delete static_cast<UniformData<sf::Glsl::Mat3>*>(uniform.Data); break;
			case GL_FLOAT_MAT4:			delete static_cast<UniformData<sf::Glsl::Mat4>*>(uniform.Data); break;

				// I will come back to textures
				// case GL_SAMPLER_2D:			delete static_cast<UniformData<float>*>(uniform.Data); break;

			default:					LOG_CORE_ERROR("Shader uniform with unknown data type."); break;
			}
		}

		m_Uniforms.clear();
	}

	sf::Shader* Material::Bind()
	{

		// get a pointer to the shader object from the resource manager
		sf::Shader* shaderHandle = ResourceManager::GetResourceHandle<sf::Shader>(m_ShaderResourceID);

		for (Uniform& uniform : m_Uniforms)
		{
			
			// to retrieve the data from the uniform we have to do some casting magic
			// we know the data type of the uniform: we have it stored as a GLenum
			// so we switch on the type to find out which type it is

			// then we need to cast the void* back to a pointer of UniformData of that type
			// only then can we pass the data into setUniform as the correct type

			switch (uniform.Type)
			{
			case GL_FLOAT:				shaderHandle->setUniform(uniform.Name, static_cast<UniformData<float>*>(uniform.Data)->Data); break;
			case GL_FLOAT_VEC2:			shaderHandle->setUniform(uniform.Name, static_cast<UniformData<sf::Glsl::Vec2>*>(uniform.Data)->Data); break;
			case GL_FLOAT_VEC3:			shaderHandle->setUniform(uniform.Name, static_cast<UniformData<sf::Glsl::Vec3>*>(uniform.Data)->Data); break;
			case GL_FLOAT_VEC4:			shaderHandle->setUniform(uniform.Name, static_cast<UniformData<sf::Glsl::Vec4>*>(uniform.Data)->Data); break;
			case GL_INT:				shaderHandle->setUniform(uniform.Name, static_cast<UniformData<int>*>(uniform.Data)->Data); break;
			case GL_INT_VEC2:			shaderHandle->setUniform(uniform.Name, static_cast<UniformData<sf::Glsl::Ivec2>*>(uniform.Data)->Data); break;
			case GL_INT_VEC3:			shaderHandle->setUniform(uniform.Name, static_cast<UniformData<sf::Glsl::Ivec3>*>(uniform.Data)->Data); break;
			case GL_INT_VEC4:			shaderHandle->setUniform(uniform.Name, static_cast<UniformData<sf::Glsl::Ivec4>*>(uniform.Data)->Data); break;
			case GL_BOOL:				shaderHandle->setUniform(uniform.Name, static_cast<UniformData<bool>*>(uniform.Data)->Data); break;
			case GL_BOOL_VEC2:			shaderHandle->setUniform(uniform.Name, static_cast<UniformData<sf::Glsl::Bvec2>*>(uniform.Data)->Data); break;
			case GL_BOOL_VEC3:			shaderHandle->setUniform(uniform.Name, static_cast<UniformData<sf::Glsl::Bvec3>*>(uniform.Data)->Data); break;
			case GL_BOOL_VEC4:			shaderHandle->setUniform(uniform.Name, static_cast<UniformData<sf::Glsl::Bvec4>*>(uniform.Data)->Data); break;
			case GL_FLOAT_MAT3:			shaderHandle->setUniform(uniform.Name, static_cast<UniformData<sf::Glsl::Mat3>*>(uniform.Data)->Data); break;
			case GL_FLOAT_MAT4:			shaderHandle->setUniform(uniform.Name, static_cast<UniformData<sf::Glsl::Mat4>*>(uniform.Data)->Data); break;

			// I will come back to textures
			// case GL_SAMPLER_2D:			shaderHandle->setUniform(uniform.Name, static_cast<UniformData<float>*>(uniform.Data)->Data); break;

			default:					LOG_CORE_ERROR("Shader uniform with unknown data type."); break;
			}
		}

		// return the shader handle so it can be passed into the renderer
		return shaderHandle;
	}


	bool Material::UniformExists(const std::string& name)
	{
		for (const Uniform& uniform : m_Uniforms)
		{
			if (uniform.Name == name)
				return true;
		}

		return false;
	}

	Uniform& Material::GetUniform(const std::string& name)
	{
		for (Uniform& uniform : m_Uniforms)
		{
			if (uniform.Name == name)
				return uniform;
		}
		// this should never be reached provided UniformExists check was carried out first
		LOG_CORE_ERROR("Uniform {0} doesn't exist: Call Material::UniformExists before attempting to retrieve uniforms.", name);
	}




	ResourceID Material::Create(const std::string& shader)
	{
		// to be assigned the id of the material
		ResourceID newID = NULL_RESOURCE_ID;

		// check to see if a material with this shader already exists
		if (s_Materials.find(shader) != s_Materials.end())
		{
			// a material with this shader already exists
			// we should find the resource ID of it and return that
			newID = s_Materials.at(shader);
		}
		else
		{
			// a material using this shader has not been created before
			// we must create a new material
			Material* newMat = new Material(shader);

			// this new material should be managed by the resource manager
			// register this object with the resource manager
			// this gives us the resource ID of the material
			newID = ResourceManager::ManageResource(newMat);

			// now that we have created a material, we cache it into the map
			// so that it can be shared between sprites that want the same material
			s_Materials.insert(std::make_pair(shader, newID));
		}

		return newID;
	}


	void Material::DestroyAllCached()
	{
		for (auto& mat : s_Materials)
		{
			ResourceManager::DeleteResource<Material>(mat.second);
		}
		s_Materials.clear();
	}


}
