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

		void Update()
		{
			for (auto const& entity : m_Entities)
			{
				auto& sRenderer = m_Coordinator->GetComponent<SpriteRenderer>(entity);
				auto& transform = m_Coordinator->GetComponent<Transform>(entity);

				sRenderer.Sprite.setPosition(transform.Position);
				sRenderer.Sprite.setRotation(transform.Rotation);
				sRenderer.Sprite.setScale(transform.Scale);
			}
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


			// we do not want to divide by 0
			float normalizeFactor = 1 / (m_MaxOrderInLayer == 0 ? 1.0f : (float)m_MaxOrderInLayer);


			for (const auto& c : components)
			{
				// get the material
				Material* mat = c.GetMaterial();

				// set shader uniforms
				mat->SetUniform("u_DepthValue", (float)c.OrderInLayer * normalizeFactor);
				sf::Shader* shader = mat->Bind();

				m_RenderWindow->draw(c.Sprite, shader);
			}

			m_RenderWindow->setActive(false);
		
		}
	private:
		Coordinator* m_Coordinator = nullptr;
		sf::RenderWindow* m_RenderWindow = nullptr;

		int m_MaxOrderInLayer = 0;
	};

}
