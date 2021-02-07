#include "Material.h"


#include <Tracy.hpp>

namespace SFMLEngine {

	std::vector<MaterialData> Material::s_MaterialCache;
	bool Material::s_WarnOnUnknownUniform = true;

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

		const GLsizei bufferSize = 48; // maximum name length
		GLchar uniformName[bufferSize]; // variable name in GLSL
		GLsizei length; // name length

		// get the number of uniforms used in the shader
		glGetProgramiv(nativeShaderHandle, GL_ACTIVE_UNIFORMS, &uniformCount);

		for (GLuint i = 0; i < (GLuint)uniformCount; i++)
		{
			// get the information about the current uniform
			glGetActiveUniform(nativeShaderHandle, i, bufferSize, &length, &size, &type, uniformName);

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

					// there are some special case uniforms that we dont expose to materials
					// but are instead handled directly by the render system
					if (newUniform.Name == "u_DepthValue" || newUniform.Name == "u_NormalMap")
						continue;

					// store the type of the uniform
					// this is used for retrieving the data later
					newUniform.Type = type;
					// a pointer to the data the uniform contains
					// this is void* since we dont know what data type it will be in advance
					// but we can use the Type attribute to retrieve it to the correct data type

					// assign it some default data in case it isnt done manually in the program
					// and we dont want to try pass nullptr into the shader
					switch (type)
					{
					case GL_FLOAT:				newUniform.Data = (void*)new UniformData(0.0f); break;
					case GL_FLOAT_VEC2:			newUniform.Data = (void*)new UniformData(sf::Vector2f(0.0f, 0.0f)); break;
					case GL_FLOAT_VEC3:			newUniform.Data = (void*)new UniformData(sf::Vector3f(0.0f, 0.0f, 0.0f)); break;
					case GL_FLOAT_VEC4:			newUniform.Data = (void*)new UniformData(sf::Color::White); break;
					case GL_INT:				newUniform.Data = (void*)new UniformData(0); break;
					case GL_INT_VEC2:			newUniform.Data = (void*)new UniformData(sf::Vector2i(0, 0)); break;
					case GL_INT_VEC3:			newUniform.Data = (void*)new UniformData(sf::Vector3i(0, 0, 0)); break; 
					case GL_INT_VEC4:			newUniform.Data = (void*)new UniformData(sf::Glsl::Ivec4(0, 0, 0, 0)); break;
					case GL_BOOL:				newUniform.Data = (void*)new UniformData(false); break;
					case GL_BOOL_VEC2:			newUniform.Data = (void*)new UniformData(sf::Glsl::Bvec2(false, false)); break;
					case GL_BOOL_VEC3:			newUniform.Data = (void*)new UniformData(sf::Glsl::Bvec3(false, false, false)); break;
					case GL_BOOL_VEC4:			newUniform.Data = (void*)new UniformData(sf::Glsl::Bvec4(false, false, false, false)); break;
					case GL_FLOAT_MAT3:			newUniform.Data = (void*)new UniformData(sf::Glsl::Mat3(sf::Transform())); break;
					case GL_FLOAT_MAT4:			newUniform.Data = (void*)new UniformData(sf::Glsl::Mat4(sf::Transform())); break;
					case GL_SAMPLER_2D:			newUniform.Data = (void*)new UniformData(NULL_RESOURCE_ID); break;

					default:					LOG_CORE_ERROR("Shader uniform with unknown data type."); break;
					}

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
			// we need to cast the uniform data to the right type so that the destructor is called
			switch (uniform.Type)
			{
			case GL_FLOAT:				delete static_cast<UniformData<float>*>(uniform.Data); break;
			case GL_FLOAT_VEC2:			delete static_cast<UniformData<sf::Vector2f>*>(uniform.Data); break;
			case GL_FLOAT_VEC3:			delete static_cast<UniformData<sf::Vector3f>*>(uniform.Data); break;
			case GL_FLOAT_VEC4:			delete static_cast<UniformData<sf::Color>*>(uniform.Data); break;
			case GL_INT:				delete static_cast<UniformData<int>*>(uniform.Data); break;
			case GL_INT_VEC2:			delete static_cast<UniformData<sf::Vector2i>*>(uniform.Data); break;
			case GL_INT_VEC3:			delete static_cast<UniformData<sf::Vector3i>*>(uniform.Data); break;
			case GL_INT_VEC4:			delete static_cast<UniformData<sf::Glsl::Ivec4>*>(uniform.Data); break;
			case GL_BOOL:				delete static_cast<UniformData<bool>*>(uniform.Data); break;
			case GL_BOOL_VEC2:			delete static_cast<UniformData<sf::Glsl::Bvec2>*>(uniform.Data); break;
			case GL_BOOL_VEC3:			delete static_cast<UniformData<sf::Glsl::Bvec3>*>(uniform.Data); break;
			case GL_BOOL_VEC4:			delete static_cast<UniformData<sf::Glsl::Bvec4>*>(uniform.Data); break;
			case GL_FLOAT_MAT3:			delete static_cast<UniformData<sf::Glsl::Mat3>*>(uniform.Data); break;
			case GL_FLOAT_MAT4:			delete static_cast<UniformData<sf::Glsl::Mat4>*>(uniform.Data); break;
			case GL_SAMPLER_2D:			delete static_cast<UniformData<ResourceID>*>(uniform.Data); break;

			default:					LOG_CORE_ERROR("Shader uniform with unknown data type."); break;
			}
		}

		m_Uniforms.clear();
	}

	sf::Shader* Material::SetUniforms()
	{
		ZoneScoped;
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
			case GL_FLOAT:		shaderHandle->setUniform(uniform.Name, static_cast<UniformData<float>*>(uniform.Data)->Data); break;
			case GL_FLOAT_VEC2:	shaderHandle->setUniform(uniform.Name, sf::Glsl::Vec2(static_cast<UniformData<sf::Vector2f>*>(uniform.Data)->Data)); break;
			case GL_FLOAT_VEC3:	shaderHandle->setUniform(uniform.Name, sf::Glsl::Vec3(static_cast<UniformData<sf::Vector3f>*>(uniform.Data)->Data)); break;
			case GL_FLOAT_VEC4:	shaderHandle->setUniform(uniform.Name, sf::Glsl::Vec4(static_cast<UniformData<sf::Color>*>(uniform.Data)->Data)); break;
			case GL_INT:		shaderHandle->setUniform(uniform.Name, static_cast<UniformData<int>*>(uniform.Data)->Data); break;
			case GL_INT_VEC2:	shaderHandle->setUniform(uniform.Name, sf::Glsl::Ivec2(static_cast<UniformData<sf::Vector2i>*>(uniform.Data)->Data)); break;
			case GL_INT_VEC3:	shaderHandle->setUniform(uniform.Name, sf::Glsl::Ivec3(static_cast<UniformData<sf::Vector3i>*>(uniform.Data)->Data)); break;
			case GL_INT_VEC4:	shaderHandle->setUniform(uniform.Name, static_cast<UniformData<sf::Glsl::Ivec4>*>(uniform.Data)->Data); break;
			case GL_BOOL:		shaderHandle->setUniform(uniform.Name, static_cast<UniformData<bool>*>(uniform.Data)->Data); break;
			case GL_BOOL_VEC2:	shaderHandle->setUniform(uniform.Name, static_cast<UniformData<sf::Glsl::Bvec2>*>(uniform.Data)->Data); break;
			case GL_BOOL_VEC3:	shaderHandle->setUniform(uniform.Name, static_cast<UniformData<sf::Glsl::Bvec3>*>(uniform.Data)->Data); break;
			case GL_BOOL_VEC4:	shaderHandle->setUniform(uniform.Name, static_cast<UniformData<sf::Glsl::Bvec4>*>(uniform.Data)->Data); break;
			case GL_FLOAT_MAT3:	shaderHandle->setUniform(uniform.Name, static_cast<UniformData<sf::Glsl::Mat3>*>(uniform.Data)->Data); break;
			case GL_FLOAT_MAT4:	shaderHandle->setUniform(uniform.Name, static_cast<UniformData<sf::Glsl::Mat4>*>(uniform.Data)->Data); break;
			case GL_SAMPLER_2D:	shaderHandle->setUniform(uniform.Name,
				*ResourceManager::GetResourceHandle<sf::Texture>(static_cast<UniformData<ResourceID>*>(uniform.Data)->Data)); break;

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
		// check to see if theres a cached material using the same shader
		// if there is then return the resource id of that
		ResourceID cachedMaterial = MaterialCached(shader);
		if (cachedMaterial != NULL_RESOURCE_ID)
			return cachedMaterial;

		// we do not have a shared material with the same shader already cached
		
		// a material using this shader has not been created before
		// we must create a new material
		Material* newMat = new Material(shader);

		// this new material should be managed by the resource manager
		// register this object with the resource manager
		// this gives us the resource ID of the material
		ResourceID newID = ResourceManager::ManageResource(newMat);

		// now that we have created a material, we cache it into the map
		// so that it can be shared between sprites that want the same material
		s_MaterialCache.push_back(MaterialData{ shader, newID, true, ShaderLibrary::IsLitShader(shader) });

		return newID;
	}

	ResourceID Material::CreateInstance(const std::string& shader)
	{
		// do not check the cache

		Material* newMat = new Material(shader);

		// this new material should be managed by the resource manager
		// register this object with the resource manager
		// this gives us the resource ID of the material
		ResourceID newID = ResourceManager::ManageResource(newMat);

		// register this material in the material cache
		// but then state that it is not to be shared with other sprites
		s_MaterialCache.push_back(MaterialData{ shader, newID, false, ShaderLibrary::IsLitShader(shader) });

		return newID;
	}

	ResourceID Material::MaterialCached(const std::string& shaderName)
	{
		for (auto const& mat : s_MaterialCache)
		{
			if (mat.ShaderName == shaderName && mat.Shared)
				return mat.MaterialID;
		}

		return NULL_RESOURCE_ID;
	}

	void Material::DestroyAllCached()
	{
		for (auto& mat : s_MaterialCache)
		{
			ResourceManager::DeleteResource<Material>(mat.MaterialID);
		}
		s_MaterialCache.clear();
	}


}
