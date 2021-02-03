#include "LightingSystem.h"


namespace SFMLEngine {

	int LightingSystem::GetLightingData(LightData* lightArray)
	{
		if (m_Entities.size() < MAX_LIGHTS)
		{
			// we have not exceeded the maximum number of lights the shader supports
			// we can just submit all light components to render
			int index = 0;
			for (const auto& entity : m_Entities)
			{
				PointLight light = m_Coordinator->GetComponent<PointLight>(entity);
				Transform transform = m_Coordinator->GetComponent<Transform>(entity);
				lightArray[index] = LightData{ sf::Vector3f({transform.Position.x, transform.Position.y, 5}), light.Intensity, light.Range, light.Color };
				++index;
			}
			return index;
		}

		return 0;
	}

}