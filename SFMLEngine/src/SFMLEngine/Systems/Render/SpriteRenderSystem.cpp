#include "SpriteRenderSystem.h"

#include "SFMLEngine/Math.h"


namespace SFMLEngine {

	void SpriteRenderSystem::Init(Coordinator* coordinator, sf::RenderWindow* window)
	{
		m_Coordinator = coordinator;
		m_RenderWindow = window;
	}


	void SpriteRenderSystem::EntityAddedToSystem(Entity entity)
	{
		SpriteRenderer* sRenderer = &m_Coordinator->GetComponent<SpriteRenderer>(entity);
		Transform* transform = &m_Coordinator->GetComponent<Transform>(entity);
		
		if (sRenderer->TextureHandle != NULL_RESOURCE_ID)
		{
			sRenderer->Sprite.setTexture(*ResourceManager::GetResourceHandle<sf::Texture>(sRenderer->TextureHandle));
			
		}
		else
		{
			LOG_WARN("No texture was supplied for entity ID {0}", entity);
		}

		m_MaxOrderInLayer = std::max(m_MaxOrderInLayer, abs(sRenderer->OrderInLayer));
		m_MaxRenderLayer = std::max(m_MaxRenderLayer, abs(sRenderer->RenderLayer));
		m_OrderInLayerNormalizeFactor = m_MaxOrderInLayer == 0 ? 0 : 1.0f / (float)(m_MaxOrderInLayer + 1.0f);
		m_RenderLayerNormaizeFactor = m_MaxRenderLayer == 0 ? 1.0f : 1.0f / (float)(m_MaxRenderLayer);

		m_SpriteRenderers.insert(std::make_pair(entity, sRenderer));
		m_Transforms.insert(std::make_pair(entity, transform));
	}

	void SpriteRenderSystem::EntityRemovedFromSystem(Entity entity)
	{
		m_SpriteRenderers.erase(entity);
		m_Transforms.erase(entity);
	}

	void SpriteRenderSystem::Update()
	{
		ZoneScoped;

		bool anyChanged = false;
		for (auto& e : m_Entities)
		{
			SpriteRenderer* spriteRenderer = m_SpriteRenderers[e];
			if (ComponentModified(spriteRenderer))
			{
				// if a sprite renderer has been modified then we need to update the order in layer factors etc
				m_MaxOrderInLayer = std::max(m_MaxOrderInLayer, abs(spriteRenderer->OrderInLayer));
				m_MaxRenderLayer = std::max(m_MaxRenderLayer, abs(spriteRenderer->RenderLayer));
				anyChanged = true;

				ResetModified(spriteRenderer);
			}
		}

		if (anyChanged)
		{
			m_OrderInLayerNormalizeFactor = m_MaxOrderInLayer == 0 ? 0 : 1.0f / (float)(m_MaxOrderInLayer + 1.0f);
			m_RenderLayerNormaizeFactor = m_MaxRenderLayer == 0 ? 1.0f : 1.0f / (float)(m_MaxRenderLayer);
		}
	}

	void SpriteRenderSystem::Render()
	{
		ZoneScoped;

		if (!m_Entities.size()) return;

		for (const auto& entity : m_Entities)
		{
			ZoneScoped;
			ZoneName("DrawSprite", 10);
			SpriteRenderer* sR = m_SpriteRenderers[entity];
			Transform* t = m_Transforms[entity];
			
			sf::Shader* shader = sR->MaterialPtr->GetShaderPtr();
			

			{
				ZoneScoped;
				ZoneName("SetDepth", 8);
				// set shader uniforms
				float depth = (sR->RenderLayer + (sR->OrderInLayer * m_OrderInLayerNormalizeFactor)) * m_RenderLayerNormaizeFactor;
				shader->setUniform("u_DepthValue", depth);
			}
			
			if (sR->MaterialPtr->IsLit())
			{
				ZoneScoped;
				ZoneName("SetLight", 8);
				shader->setUniform("u_NormalMap", *sR->NormalMapPtr);
					
				// rotation value is used to compute transformed normals so lighting is correct for rotated sprites
				// requires negated because of the y axis being flipped
				shader->setUniform("u_Rotation", -t->Rotation * Math::DEG_TO_RAD);
					
			}
				
			// create a transform
			m_RenderState.transform = t->GetWorldTransformMatrix();
			m_RenderState.shader = shader;
				
			{
				ZoneScoped;
				ZoneName("Draw", 4);
				m_RenderWindow->draw(sR->Sprite, m_RenderState);
			}
		}
		
	}
}
