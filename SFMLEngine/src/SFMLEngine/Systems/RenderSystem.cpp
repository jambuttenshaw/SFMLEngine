#include "RenderSystem.h"

namespace SFMLEngine {


	void RenderSystem::EntityAddedToSystem(Entity entity)
	{
		auto& sRenderer = m_Coordinator->GetComponent<SpriteRenderer>(entity);
		if (sRenderer.TextureHandle != NULL_RESOURCE_ID)
		{
			sRenderer.Sprite.setTexture(*ResourceManager::GetResourceHandle<sf::Texture>(sRenderer.TextureHandle));
		}
		else
		{
			LOG_WARN("No texture was supplied for entity ID {0}", entity);
		}
		m_MaxOrderInLayer = std::max(m_MaxOrderInLayer, abs(sRenderer.OrderInLayer));
		m_MaxRenderLayer = std::max(m_MaxRenderLayer, abs(sRenderer.RenderLayer));
	}


	void RenderSystem::Render()
	{
		// get the sprite renderer components
		auto components = m_Coordinator->GetComponents<SpriteRenderer>(m_Entities);


		// set the context active
		m_RenderWindow->setActive();

		// set up the renderer for drawing
		Renderer::SetOpenGLStates();
		Renderer::Begin();


		// create a renderstate to choose how the sprite is rendered
		sf::RenderStates renderState;


		// we do not want to divide by 0
		float orderInLayerNormalizeFactor = m_MaxOrderInLayer == 0 ? 0 : 1.0f / (float)(m_MaxOrderInLayer + 1.0f);
		float renderLayerNormaizeFactor = m_MaxRenderLayer == 0 ? 1.0f : 1.0f / (float)(m_MaxRenderLayer);


		for (const auto& entity : m_Entities)
		{
			auto const& sR = m_Coordinator->GetComponent<SpriteRenderer>(entity);
			auto const& t = m_Coordinator->GetComponent<Transform>(entity);

			// get the material
			Material* mat = sR.GetMaterial();

			// set shader uniforms
			float depth = (sR.RenderLayer + (sR.OrderInLayer * orderInLayerNormalizeFactor)) * renderLayerNormaizeFactor;
			mat->SetUniform("u_DepthValue", depth);

			sf::Shader* shader = mat->SetUniforms();

			renderState.shader = shader;

			// create a transform
			sf::Transform transform;
			transform.translate(t.Position);
			transform.rotate(t.Rotation);
			transform.scale(t.Scale);
			renderState.transform = transform;

			m_RenderWindow->draw(sR.Sprite, renderState);
		}

		m_RenderWindow->setActive(false);

	}

}
