#include "DirectionalLightSystem.h"

#include "../../ECS/Components/DirectionalLight.h"
#include "../../Renderer/Material.h"

#include <Tracy.hpp>

namespace SFMLEngine {

	void DirectionalLightSystem::EntityAddedToSystem(Entity entity)
	{
		if (IsStatic(entity))
		{
			m_StaticLightCount++;
			UploadStaticLight(entity, m_StaticLightCount - 1);
		}
		else
		{
			m_LightCount++;
			UploadLightCountData();
		}
	}

	void DirectionalLightSystem::EntityRemovedFromSystem(Entity entity)
	{
		if (IsStatic(entity))
		{
			m_StaticLightCount--;
			// need to completely reload the lighting data when a static light is destroyed
			// to ensure the lighting data array in the shader is tightly packed
			UploadLightCountData();
			UploadAllLightingData();
		}
		else
		{
			m_LightCount--;
			UploadLightCountData();
		}
	}

	void DirectionalLightSystem::UploadAllLightingData()
	{
		ZoneScoped;
		if (m_LightCount + m_StaticLightCount < MAX_DIRECTIONAL_LIGHTS)
		{
			for (auto const& entity : m_Entities)
			{
				DirectionalLight& light = m_Coordinator->GetComponent<DirectionalLight>(entity);
				if (!ComponentModified(light) || light.Static) continue;

				for (const auto& matData : Material::GetAllMaterialsInUse())
				{
					if (!matData.Lit) continue;

					Material* mat = ResourceManager::GetResourceHandle<Material>(matData.MaterialID);
					for (int i = m_StaticLightCount; i < m_LightCount + m_StaticLightCount; i++)
					{
						std::string lightIndex("u_DirectionalLights[" + std::to_string(i) + "]");
						mat->SetProperty(lightIndex + ".Direction", light.Direction);
						mat->SetProperty(lightIndex + ".Intensity", light.Intensity);
						mat->SetProperty(lightIndex + ".Color", light.Color);
					}
				}

				ResetModified(light);

			}
		}
		else
		{
			LOG_CORE_ERROR("Too many lights in the scene! Cannot submit lighting data to shader.");
		}
	}

	void DirectionalLightSystem::UploadLightCountData()
	{
		if (m_LightCount + m_StaticLightCount < MAX_DIRECTIONAL_LIGHTS)
		{
			for (const auto& matData : Material::GetAllMaterialsInUse())
			{
				if (!matData.Lit) continue;

				Material* mat = ResourceManager::GetResourceHandle<Material>(matData.MaterialID);
				mat->SetProperty("u_NumDirectionalLights", m_LightCount + m_StaticLightCount);
			}
		}
		else
		{
			LOG_CORE_ERROR("Too many lights in the scene! Cannot submit lighting data to shader.");
		}
	}

	bool DirectionalLightSystem::IsStatic(Entity entity)
	{
		return m_Coordinator->GetComponent<DirectionalLight>(entity).Static;
	}

	void DirectionalLightSystem::UploadStaticLight(Entity entity, int index)
	{
		SFMLE_CORE_ASSERT(index < MAX_DIRECTIONAL_LIGHTS, "Invalid light index!");

		DirectionalLight light = m_Coordinator->GetComponent<DirectionalLight>(entity);

		for (const auto& matData : Material::GetAllMaterialsInUse())
		{
			if (!matData.Lit) continue;

			Material* mat = ResourceManager::GetResourceHandle<Material>(matData.MaterialID);

			// update the number of lights
			mat->SetProperty("u_NumDirectionalLights", m_LightCount + m_StaticLightCount);

			std::string lightIndex("u_DirectionalLights[" + std::to_string(index) + "]");
			mat->SetProperty(lightIndex + ".Direction", light.Direction);
			mat->SetProperty(lightIndex + ".Intensity", light.Intensity);
			mat->SetProperty(lightIndex + ".Color", light.Color);
		}
	}
}