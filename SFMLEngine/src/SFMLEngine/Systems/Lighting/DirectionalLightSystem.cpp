#include "DirectionalLightSystem.h"

#include "../../Renderer/Material.h"


namespace SFMLEngine {

	void DirectionalLightSystem::UploadAllLightingData()
	{
		if (m_LightCount < MAX_DIRECTIONAL_LIGHTS)
		{
			for (auto const& entity : m_Entities)
			{
				DirectionalLight light = m_Coordinator->GetComponent<DirectionalLight>(entity);

				for (const auto& matData : Material::GetAllMaterialsInUse())
				{
					if (!matData.Lit) continue;

					Material* mat = ResourceManager::GetResourceHandle<Material>(matData.MaterialID);
					for (int i = 0; i < m_LightCount; i++)
					{
						std::string lightIndex("u_DirectionalLights[" + std::to_string(i) + "]");
						mat->SetProperty(lightIndex + ".Direction", light.Direction);
						mat->SetProperty(lightIndex + ".Intensity", light.Intensity);
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

	void DirectionalLightSystem::UploadLightCountData()
	{
		if (m_LightCount < MAX_DIRECTIONAL_LIGHTS)
		{
			for (const auto& matData : Material::GetAllMaterialsInUse())
			{
				if (!matData.Lit) continue;

				Material* mat = ResourceManager::GetResourceHandle<Material>(matData.MaterialID);
				mat->SetProperty("u_NumDirectionalLights", m_LightCount);
			}
		}
		else
		{
			LOG_CORE_ERROR("Too many lights in the scene! Cannot submit lighting data to shader.");
		}
	}
}