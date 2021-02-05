#include "PointLightSystem.h"

#include "../../Renderer/Material.h"


namespace SFMLEngine {

	void PointLightSystem::UploadAllLightingData()
	{
		if (m_LightCount < MAX_POINT_LIGHTS)
		{
			for (auto const& entity : m_Entities)
			{
				PointLight light = m_Coordinator->GetComponent<PointLight>(entity);
				Transform transform = m_Coordinator->GetComponent<Transform>(entity);

				for (const auto& matData : Material::GetAllMaterialsInUse())
				{
					if (!matData.Lit) continue;

					Material* mat = ResourceManager::GetResourceHandle<Material>(matData.MaterialID);
					for (int i = 0; i < m_LightCount; i++)
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
		if (m_LightCount < MAX_POINT_LIGHTS)
		{
			for (const auto& matData : Material::GetAllMaterialsInUse())
			{
				if (!matData.Lit) continue;

				Material* mat = ResourceManager::GetResourceHandle<Material>(matData.MaterialID);
				mat->SetProperty("u_NumPointLights", m_LightCount);
			}
		}
		else
		{
			LOG_CORE_ERROR("Too many lights in the scene! Cannot submit lighting data to shader.");
		}
	}
}