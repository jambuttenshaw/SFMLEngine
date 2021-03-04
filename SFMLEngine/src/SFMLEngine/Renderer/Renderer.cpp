#include "Renderer.h"

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include "SFMLEngine/Core.h"

#include "Material.h"

#include "SFMLEngine/Systems/Render/SpriteRenderSystem.h"
#include "SFMLEngine/Systems/Render/TilemapRenderSystem.h"

namespace SFMLEngine {

	sf::ContextSettings* Renderer::s_ContextSettings = nullptr;
	std::shared_ptr<SpriteRenderSystem> Renderer::s_SpriteRenderSystem = nullptr;
	std::shared_ptr<TilemapRenderSystem> Renderer::s_TilemapRenderSystem = nullptr;

	const sf::ContextSettings& Renderer::Init()
	{
		ZoneScoped;
		ZoneName("RendererBegin", 13);

		// create the context settings
		s_ContextSettings = new sf::ContextSettings();
		s_ContextSettings->depthBits = 24;
		s_ContextSettings->majorVersion = 4;
		s_ContextSettings->minorVersion = 6;

		// Output the context settings and shader capabilities of the hardware
		LOG_CORE_INFO("OpenGL Version: {0}.{1}", s_ContextSettings->majorVersion, s_ContextSettings->minorVersion);
		LOG_CORE_INFO("Shaders Supported: {0}", (sf::Shader::isAvailable() ? "Yes" : "No"));

		return *s_ContextSettings;
	}

	void Renderer::InitGLEW()
	{
		ZoneScoped;

		// initialise glew
		// this must be after a valid openGL context is created
		GLenum err = glewInit();
		SFMLE_CORE_ASSERT(err == GLEW_OK, "GLEW failed to initialise.");
	}

	void Renderer::SetRenderSystems(std::shared_ptr<SpriteRenderSystem> sRS, std::shared_ptr<TilemapRenderSystem> tRS)
	{
		s_SpriteRenderSystem = sRS;
		s_TilemapRenderSystem = tRS;
	}

	void Renderer::Shutdown()
	{
		delete s_ContextSettings;
	}

	void Renderer::Render()
	{
		ZoneScoped;
		
		auto const& layers1 = s_SpriteRenderSystem->GetRenderLayersUsed();
		auto const& layers2 = s_TilemapRenderSystem->GetRenderLayersUsed();
		std::vector<int> out(layers1.size() + layers2.size());

		auto it = std::set_union(layers1.begin(), layers1.end(), layers2.begin(), layers2.end(), out.begin());
		out.resize(it - out.begin());
		
		for (int layer : out)
		{
			for (auto const& materialData : Material::GetAllMaterialsInUse())
			{
				// set the shader uniforms (with the exception of the depth value) once per material, rather than once per sprite
				materialData.MaterialPtr->SetUniforms();

				s_SpriteRenderSystem->Render(materialData.MaterialID, layer);
				s_TilemapRenderSystem->Render(materialData.MaterialID, layer);
			}
		}
	}
}
