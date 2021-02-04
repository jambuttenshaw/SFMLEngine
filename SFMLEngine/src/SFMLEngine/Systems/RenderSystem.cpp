#include "RenderSystem.h"

namespace SFMLEngine {

	void RenderSystem::Init(Coordinator* coordinator, sf::RenderWindow* window)
	{
		m_Coordinator = coordinator;
		m_RenderWindow = window;

		Material::GetAllMaterialsInUse(m_Materials);

		// lighting systems
		m_PointLightSystem = m_Coordinator->RegisterSystem<PointLightSystem>();
		m_PointLightSystem->Init(coordinator);
		{
			Signature signature;
			signature.set(m_Coordinator->GetComponentType<Transform>());
			signature.set(m_Coordinator->GetComponentType<PointLight>());
			m_Coordinator->SetSystemSignature<PointLightSystem>(signature);
		}
		m_DirectionalLightSystem = m_Coordinator->RegisterSystem<DirectionalLightSystem>();
		m_DirectionalLightSystem->Init(coordinator);
		{
			Signature signature;
			signature.set(m_Coordinator->GetComponentType<DirectionalLight>());
			m_Coordinator->SetSystemSignature<DirectionalLightSystem>(signature);
		}
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

		// update the materials that the system knows about
		Material::GetAllMaterialsInUse(m_Materials);
	}


	void RenderSystem::Render()
	{
		// retrieve lighting data from lighting system
		m_NumPointLights = m_PointLightSystem->GetLightingData(m_PointLights);
		m_NumDirectionalLights = m_DirectionalLightSystem->GetLightingData(m_DirectionalLights);

		for (const auto& materialData : m_Materials)
		{
			// get all the entities that use that material
			// this populates m_CurrentEntities with the entities using this material
			GetAllEntitiesWithMaterial(materialData);

			// set the shader uniforms (with the exception of the depth value) once per material, rather than once per sprite
			Material* mat = ResourceManager::GetResourceHandle<Material>(materialData.MaterialID);

			// if it is a lit material we want to upload all of the lighting uniforms
			if (materialData.Lit)
				UploadLightingData(mat);

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

	void RenderSystem::UploadLightingData(Material* material)
	{
		material->SetProperty("u_NumPointLights", m_NumPointLights);
		for (int i = 0; i < m_NumPointLights; i++)
		{
			std::string lightIndex("u_PointLights[" + std::to_string(i) + "]");
			material->SetProperty(lightIndex + ".Position", m_PointLights[i].Position);
			material->SetProperty(lightIndex + ".Intensity", m_PointLights[i].Intensity);
			material->SetProperty(lightIndex + ".Range", m_PointLights[i].Range);
			material->SetProperty(lightIndex + ".Color", m_PointLights[i].Color);
		}


		material->SetProperty("u_NumDirectionalLights", m_NumDirectionalLights);
		for (int i = 0; i < m_NumDirectionalLights; i++)
		{
			std::string lightIndex("u_DirectionalLights[" + std::to_string(i) + "]");
			material->SetProperty(lightIndex + ".Direction", m_DirectionalLights[i].Direction);
			material->SetProperty(lightIndex + ".Intensity", m_DirectionalLights[i].Intensity);
			material->SetProperty(lightIndex + ".Color", m_DirectionalLights[i].Color);
		}
	}

}
