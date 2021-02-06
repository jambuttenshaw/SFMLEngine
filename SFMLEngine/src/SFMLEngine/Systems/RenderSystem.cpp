#include "RenderSystem.h"

namespace SFMLEngine {

	void RenderSystem::Init(Coordinator* coordinator, sf::RenderWindow* window)
	{
		m_Coordinator = coordinator;
		m_RenderWindow = window;
	}


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
		m_OrderInLayerNormalizeFactor = m_MaxOrderInLayer == 0 ? 0 : 1.0f / (float)(m_MaxOrderInLayer + 1.0f);
		m_RenderLayerNormaizeFactor = m_MaxRenderLayer == 0 ? 1.0f : 1.0f / (float)(m_MaxRenderLayer);
	}

	void RenderSystem::Update()
	{
		for (auto& e : m_Entities)
		{
			SpriteRenderer& spriteRenderer = m_Coordinator->GetComponent<SpriteRenderer>(e);
			if (ComponentModified(spriteRenderer))
			{
				// if a sprite renderer has been modified then we need to update the order in layer factors etc
				m_MaxOrderInLayer = std::max(m_MaxOrderInLayer, abs(spriteRenderer.OrderInLayer));
				m_MaxRenderLayer = std::max(m_MaxRenderLayer, abs(spriteRenderer.RenderLayer));
				m_OrderInLayerNormalizeFactor = m_MaxOrderInLayer == 0 ? 0 : 1.0f / (float)(m_MaxOrderInLayer + 1.0f);
				m_RenderLayerNormaizeFactor = m_MaxRenderLayer == 0 ? 1.0f : 1.0f / (float)(m_MaxRenderLayer);

				ResetModified(spriteRenderer);
			}
		}
	}

	void RenderSystem::Render()
	{
		for (const auto& materialData : Material::GetAllMaterialsInUse())
		{
			// get all the entities that use that material
			// this populates m_CurrentEntities with the entities using this material
			GetAllEntitiesWithMaterial(materialData);

			// set the shader uniforms (with the exception of the depth value) once per material, rather than once per sprite
			Material* mat = ResourceManager::GetResourceHandle<Material>(materialData.MaterialID);

			sf::Shader* shader = mat->SetUniforms();
			m_RenderState.shader = shader;

			for (const auto& entity : m_CurrentEntities)
			{
				auto const& sR = m_Coordinator->GetComponent<SpriteRenderer>(entity);
				auto const& t = m_Coordinator->GetComponent<Transform>(entity);

				// set shader uniforms
				float depth = (sR.RenderLayer + (sR.OrderInLayer * m_OrderInLayerNormalizeFactor)) * m_RenderLayerNormaizeFactor;
				shader->setUniform("u_DepthValue", depth);
				if (materialData.Lit)
				{
					shader->setUniform("u_NormalMap", *ResourceManager::GetResourceHandle<sf::Texture>(sR.NormalMapHandle));
					// rotation value is used to compute transformed normals so lighting is correct for rotated sprites
					// requires negated because of the y axis being flipped
					shader->setUniform("u_Rotation", -t.Rotation * DEG_TO_RAD);
				}
				
				// create a transform
				m_RenderState.transform = t.GetTransformMatrix();

				m_RenderWindow->draw(sR.Sprite, m_RenderState);
			}
		}
	}

	void RenderSystem::GetAllEntitiesWithMaterial(MaterialData material)
	{
		m_CurrentEntities.clear();

		for (auto& entity : m_Entities)
		{
			if (m_Coordinator->GetComponent<SpriteRenderer>(entity).MaterialHandle == material.MaterialID)
			{
				m_CurrentEntities.push_back(entity);
				if (!material.Shared) return;
			}
		}

	}
}
