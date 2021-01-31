#pragma once

#include <SFML/Graphics.hpp>

#include "../ECS/Coordinator.h"
#include "../ECS/Components.h"

#include "../ResourceManagement/ResourceManager.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/ShaderLibrary.h"
#include "../Renderer/Material.h"


namespace SFMLEngine {

	class RenderSystem : public System
	{
	public:
		RenderSystem() = default;
		~RenderSystem() = default;
		
		void Init(Coordinator* coordinator, sf::RenderWindow* window)
		{
			m_Coordinator = coordinator;
			m_RenderWindow = window;
		}

		void EntityAddedToSystem(Entity entity) override
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
		}

		void Render()
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
			float normalizeFactor = 1 / (m_MaxOrderInLayer == 0 ? 1.0f : (float)m_MaxOrderInLayer);


			for (const auto& entity : m_Entities)
			{
				auto const& sR = m_Coordinator->GetComponent<SpriteRenderer>(entity);
				auto const& t = m_Coordinator->GetComponent<Transform>(entity);

				// get the material
				Material* mat = sR.GetMaterial();

				// set shader uniforms
				mat->SetUniform("u_DepthValue", (float)sR.OrderInLayer * normalizeFactor);
				sf::Shader* shader = mat->Bind();

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
	private:
		Coordinator* m_Coordinator = nullptr;
		sf::RenderWindow* m_RenderWindow = nullptr;

		int m_MaxOrderInLayer = 0;
	};

}
