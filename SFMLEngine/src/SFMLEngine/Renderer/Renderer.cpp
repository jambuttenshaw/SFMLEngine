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

		// create the context settings
		s_ContextSettings = new sf::ContextSettings();
		s_ContextSettings->majorVersion = 4;
		s_ContextSettings->minorVersion = 6;

		// Output the context settings and shader capabilities of the hardware
		LOG_CORE_INFO("OpenGL Version: {0}.{1}", s_ContextSettings->majorVersion, s_ContextSettings->minorVersion);
		LOG_CORE_INFO("Shaders Supported: {0}", (sf::Shader::isAvailable() ? "Yes" : "No"));

		// return the context settings for the application to use to create the window
		return *s_ContextSettings;
	}

	void Renderer::InitGLEW()
	{
		ZoneScoped;

		// initialise glew
		// this must be after a valid openGL context is created
		GLenum err = glewInit();
		// make sure it was succesful
		SFMLE_CORE_ASSERT(err == GLEW_OK, "GLEW failed to initialise.");
	}

	void Renderer::SetRenderSystems(std::shared_ptr<SpriteRenderSystem> sRS, std::shared_ptr<TilemapRenderSystem> tRS)
	{
		// assigns the pointers to the systems
		s_SpriteRenderSystem = sRS;
		s_TilemapRenderSystem = tRS;
	}

	void Renderer::Shutdown()
	{
		// delete the context settings
		delete s_ContextSettings;
	}

	void Renderer::Render()
	{
		ZoneScoped;
		
		// entities need to be drawn in the order of their render layer
		// so we need to get every render layer in use, and then iterate through them
		// draw each entity in that layer and then move onto the next layer

		// get all the layers used by the sprite render system and tilemap render system
		auto const& layers1 = s_SpriteRenderSystem->GetRenderLayersUsed();
		auto const& layers2 = s_TilemapRenderSystem->GetRenderLayersUsed();
		// a vector to hold the render layers
		std::vector<int> out(layers1.size() + layers2.size());

		// create a union of all the render layers from each system
		auto it = std::set_union(layers1.begin(), layers1.end(), layers2.begin(), layers2.end(), out.begin());
		// because the render systems might have entities that use the same layers, the sum of the individual layers
		// may be bigger than the union of their sets
		// so we resize to properly size the vector for the number of layers
		out.resize(it - out.begin());
		
		// iterate through each layer
		for (int layer : out)
		{
			// iterate for each material
			for (auto const& materialData : Material::GetAllMaterialsInUse())
			{
				// set the shader uniforms once per material, rather than once per sprite
				materialData.MaterialPtr->SetUniforms();

				// then draw all entities in this layer that use this material
				// by design, sprites and tilemaps sharing  a render layer will be drawn tilemap first, then sprite
				// if this is not desired then assign one of them a different render layer
				s_TilemapRenderSystem->Render(materialData.MaterialID, layer);
				s_SpriteRenderSystem->Render(materialData.MaterialID, layer);
			}
		}
	}
}
