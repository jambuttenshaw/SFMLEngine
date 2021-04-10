#include "DirectionalLightSystem.h"

#include "SFMLEngine/ECS/Components/DirectionalLight.h"
#include "SFMLEngine/Renderer/Material.h"


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
			int i = m_StaticLightCount;
			for (auto const& entity : m_Entities)
			{
				DirectionalLight& light = m_Coordinator->GetComponent<DirectionalLight>(entity);
				if (!ComponentModified(light) || light.IsStatic()) continue;

				for (const auto& matData : Material::GetAllMaterialsInUse())
				{
					if (!matData.Lit) continue;

					Material* mat = ResourceManager::GetResourceHandle<Material>(matData.MaterialID);

					std::string lightIndex("u_DirectionalLights[" + std::to_string(i) + "]");
					mat->SetProperty(lightIndex + ".Direction", light.GetDirection());
					mat->SetProperty(lightIndex + ".Intensity", light.GetIntensity());
					mat->SetProperty(lightIndex + ".Color", light.GetColor());
				}

				ResetModified(light);
				i++;
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
		return m_Coordinator->GetComponent<DirectionalLight>(entity).IsStatic();
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
			mat->SetProperty(lightIndex + ".Direction", light.GetDirection());
			mat->SetProperty(lightIndex + ".Intensity", light.GetIntensity());
			mat->SetProperty(lightIndex + ".Color", light.GetColor());
		}
	}
}