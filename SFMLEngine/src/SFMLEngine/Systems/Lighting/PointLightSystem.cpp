#include "PointLightSystem.h"

#include "SFMLEngine/ECS/Components/PointLight.h"
#include "SFMLEngine/ECS/Components/Transform.h"
#include "SFMLEngine/Renderer/Material.h"


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
		ZoneScoped;
		// make sure there arent too many lights
		// so we dont overflow the buffer
		if (m_LightCount + m_StaticLightCount < MAX_POINT_LIGHTS)
		{

			// loop for each entity
			int i = m_StaticLightCount;
			for (auto const& entity : m_Entities)
			{
				// get a reference to the light component on this entity
				PointLight& light = m_Coordinator->GetComponent<PointLight>(entity);
				// if it is static we dont want to reupload the light data
				if (light.Static) continue;

				// find out if the light component has been modified since last frame
				bool modified = ComponentModified(light);

				// get the transform
				Transform transform = m_Coordinator->GetComponent<Transform>(entity);

				// loop for each material
				for (const auto& matData : Material::GetAllMaterialsInUse())
				{
					// if the material is not lit we dont want to upload lighting data
					if (!matData.Lit) continue;

					// get a pointer to the current material
					Material* mat = ResourceManager::GetResourceHandle<Material>(matData.MaterialID);


					// get a string to access the point light data uniform array
					std::string lightIndex("u_PointLights[" + std::to_string(i) + "]");

					// upload the position of the light
					auto& pos = transform.GetWorldPosition();
					mat->SetProperty(lightIndex + ".Position", sf::Vector3f(pos.x, pos.y, LIGHT_Z_VALUE));

					// if the light has been modified then we want to upload the new data
					if (modified)
					{
						mat->SetProperty(lightIndex + ".Intensity", light.Intensity);
						mat->SetProperty(lightIndex + ".Range", light.Range);
						mat->SetProperty(lightIndex + ".Color", light.Color);
					}
				}
				// if the light was flagged as modified we want to reset this now that the lighting is uploaded
				if (modified) ResetModified(light);
				i++;
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
			auto& pos = transform.GetWorldPosition();
			mat->SetProperty(lightIndex + ".Position", sf::Vector3f(pos.x, pos.y, LIGHT_Z_VALUE));
			mat->SetProperty(lightIndex + ".Intensity", light.Intensity);
			mat->SetProperty(lightIndex + ".Range", light.Range);
			mat->SetProperty(lightIndex + ".Color", light.Color);
			
		}
	}
}