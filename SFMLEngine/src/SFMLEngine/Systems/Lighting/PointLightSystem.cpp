#include "PointLightSystem.h"


namespace SFMLEngine {

	int PointLightSystem::GetLightingData(PointLightData* lightArray)
	{
		if (m_Entities.size() < MAX_POINT_LIGHTS)
		{
			// we have not exceeded the maximum number of lights the shader supports
			// we can just submit all light components to render
			int index = 0;
			for (const auto& entity : m_Entities)
			{
				PointLight light = m_Coordinator->GetComponent<PointLight>(entity);
				Transform transform = m_Coordinator->GetComponent<Transform>(entity);
				lightArray[index] = PointLightData{ sf::Vector3f({transform.Position.x, transform.Position.y, 5}), light.Intensity, light.Range, light.Color };
				++index;
			}
			return index;
		}
		LOG_CORE_ERROR("Too many lights in the scene! Cannot submit lighting data to shader.");
		return 0;
	}

}