#include "PointLightSystem.h"

#include "../../Renderer/Material.h"


namespace SFMLEngine {

	void PointLightSystem::EntityAddedToSystem(Entity entity)
	{
		if (IsStatic(entity))
		{
			UploadStaticLight(entity, m_StaticLightCount);
			m_StaticLightCount++;
		}
		else
		{
			m_LightCount++;
			UploadLightCountData();
		}
	}

	void PointLightSystem::EntityRemovedFromSystem(Entity entity)
	{
		if (IsStatic(entity))
		{
			m_StaticLightCount--;
			// need to completely reload the lighting data when a static light is destroyed
			// to ensure the lighting data array in the shader is tightly packed
			UploadAllLightingData();
		}
		else
		{
			m_LightCount--;
			UploadLightCountData();
		}
	}

	void PointLightSystem::UploadAllLightingData()
	{
		if (m_LightCount + m_StaticLightCount < MAX_POINT_LIGHTS)
		{
			for (auto const& entity : m_Entities)
			{
				PointLight light = m_Coordinator->GetComponent<PointLight>(entity);
				if (light.Static) continue;

				Transform transform = m_Coordinator->GetComponent<Transform>(entity);

				for (const auto& matData : Material::GetAllMaterialsInUse())
				{
					if (!matData.Lit) continue;

					Material* mat = ResourceManager::GetResourceHandle<Material>(matData.MaterialID);
					for (int i = m_StaticLightCount; i < m_StaticLightCount + m_LightCount; i++)
					{
						std::string lightIndex("u_PointLights[" + std::to_string(i) + "]");
						mat->SetProperty(lightIndex + ".Position", sf::Vector3f(transform.Position.x, transform.Position.y, 5));
						mat->SetProperty(lightIndex + ".Intensity", light.Intensity);
						mat->SetProperty(lightIndex + ".Range", light.Range);
						mat->SetProperty(lightIndex + ".Color", light.Color);
					}
				}

			}
		}
		else
		{
			LOG_CORE_ERROR("Too many lights in the scene! Cannot submit lighting data to shader.");
		}
	}

	void PointLightSystem::UploadLightCountData()
	{
		if (m_LightCount + m_StaticLightCount < MAX_POINT_LIGHTS)
		{
			for (const auto& matData : Material::GetAllMaterialsInUse())
			{
				if (!matData.Lit) continue;

				Material* mat = ResourceManager::GetResourceHandle<Material>(matData.MaterialID);
				mat->SetProperty("u_NumPointLights", m_LightCount + m_StaticLightCount);
			}
		}
		else
		{
			LOG_CORE_ERROR("Too many lights in the scene! Cannot submit lighting data to shader.");
		}
	}

	bool PointLightSystem::IsStatic(Entity entity)
	{
		return m_Coordinator->GetComponent<PointLight>(entity).Static;
	}

	void PointLightSystem::UploadStaticLight(Entity entity, int index)
	{
		SFMLE_CORE_ASSERT(index < MAX_POINT_LIGHTS, "Invalid light index!");

		PointLight light = m_Coordinator->GetComponent<PointLight>(entity);
		Transform transform = m_Coordinator->GetComponent<Transform>(entity);

		for (const auto& matData : Material::GetAllMaterialsInUse())
		{
			if (!matData.Lit) continue;

			Material* mat = ResourceManager::GetResourceHandle<Material>(matData.MaterialID);
			
			std::string lightIndex("u_PointLights[" + std::to_string(index) + "]");
			mat->SetProperty(lightIndex + ".Position", sf::Vector3f(transform.Position.x, transform.Position.y, 5));
			mat->SetProperty(lightIndex + ".Intensity", light.Intensity);
			mat->SetProperty(lightIndex + ".Range", light.Range);
			mat->SetProperty(lightIndex + ".Color", light.Color);
			
		}
	}
}