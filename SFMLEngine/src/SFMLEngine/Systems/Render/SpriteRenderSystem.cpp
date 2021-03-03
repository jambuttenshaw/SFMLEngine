#include "SpriteRenderSystem.h"

#include "SFMLEngine/Math.h"
#include "SFMLEngine/Renderer/Renderer.h"


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
		m_OrderInLayerNormalizeFactor = Renderer::CalculateOrderInLayerFactor(m_MaxOrderInLayer);
		m_RenderLayerNormaizeFactor = Renderer::CalculateRenderLayerFactor(m_MaxRenderLayer);

		m_SpriteRenderers.insert(std::make_pair(entity, sRenderer));
		m_Transforms.insert(std::make_pair(entity, transform));

		// insert into materials map
		if (m_MaterialsMap.find(sRenderer->MaterialHandle) == m_MaterialsMap.end())
		{
			m_MaterialsMap.insert(std::make_pair(sRenderer->MaterialHandle, std::set<Entity>{ entity }));
		}
		else
		{
			m_MaterialsMap[sRenderer->MaterialHandle].insert(entity);
		}
	}

	void SpriteRenderSystem::EntityRemovedFromSystem(Entity entity)
	{
		auto mat = m_SpriteRenderers[entity]->MaterialHandle;
		m_MaterialsMap[mat].erase(entity);
		if (m_MaterialsMap[mat].empty())
			m_MaterialsMap.erase(mat);

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
			m_OrderInLayerNormalizeFactor = Renderer::CalculateOrderInLayerFactor(m_MaxOrderInLayer);
			m_RenderLayerNormaizeFactor = Renderer::CalculateRenderLayerFactor(m_MaxRenderLayer);
		}
	}

	void SpriteRenderSystem::Render(ResourceID material)
	{
		ZoneScoped;

		if (m_MaterialsMap[material].empty()) return;

		for (const auto& entity : m_MaterialsMap[material])
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
				shader->setUniform("u_DepthValue", -depth);
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
			m_RenderState.transform = t->GetLocalToWorldTransformMatrix();
			m_RenderState.shader = shader;
				
			{
				ZoneScoped;
				ZoneName("Draw", 4);
				m_RenderWindow->draw(sR->Sprite, m_RenderState);
			}
		}
		
	}
}
