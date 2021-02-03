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
		m_OrderInLayerNormalizeFactor = m_MaxOrderInLayer == 0 ? 0 : 1.0f / (float)(m_MaxOrderInLayer + 1.0f);
		m_RenderLayerNormaizeFactor = m_MaxRenderLayer == 0 ? 1.0f : 1.0f / (float)(m_MaxRenderLayer);

		// update the materials that the system knows about
		Material::GetAllMaterialsInUse(m_Materials);
	}


	void RenderSystem::Render()
	{
		// retrieve lighting data from lighting system
		m_NumLights = m_LightingSystem->GetLightingData(m_Lights);

		for (const auto& materialData : m_Materials)
		{
			// get all the entities that use that material
			// this populates m_CurrentEntities with the entities using this material
			GetAllEntitiesWithMaterial(materialData);

			// set the shader uniforms (with the exception of the depth value) once per material, rather than once per sprite
			Material* mat = ResourceManager::GetResourceHandle<Material>(materialData.MaterialID);

			// if it is a lit material we want to upload all of the lighting uniforms
			if (true)
			{
				UploadLightingData(mat);
			}

			sf::Shader* shader = mat->SetUniforms();
			m_RenderState.shader = shader;

			for (const auto& entity : m_CurrentEntities)
			{
				auto const& sR = m_Coordinator->GetComponent<SpriteRenderer>(entity);
				auto const& t = m_Coordinator->GetComponent<Transform>(entity);


				// set shader uniforms
				float depth = (sR.RenderLayer + (sR.OrderInLayer * m_OrderInLayerNormalizeFactor)) * m_RenderLayerNormaizeFactor;
				mat->SetProperty("u_DepthValue", depth);

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

	void RenderSystem::UploadLightingData(Material* material)
	{
		for (int i = 0; i < m_NumLights; i++)
		{
			std::string lightIndex("u_Lights[" + std::to_string(i) + "]");
			material->SetProperty(lightIndex + ".Position", m_Lights[i].Position);
			material->SetProperty(lightIndex + ".Intensity", m_Lights[i].Intensity);
			material->SetProperty(lightIndex + ".Range", m_Lights[i].Range);
			material->SetProperty(lightIndex + ".Color", m_Lights[i].Color);
		}
	}

}
