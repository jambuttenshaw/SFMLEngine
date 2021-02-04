#include "DirectionalLightSystem.h"


namespace SFMLEngine {

	int DirectionalLightSystem::GetLightingData(DirectionalLightData* lightArray)
	{
		if (m_Entities.size() < MAX_DIRECTIONAL_LIGHTS)
		{
			// we have not exceeded the maximum number of lights the shader supports
			// we can just submit all light components to render
			int index = 0;
			for (const auto& entity : m_Entities)
			{
				DirectionalLight light = m_Coordinator->GetComponent<DirectionalLight>(entity);
				lightArray[index] = DirectionalLightData{ light.Direction, light.Intensity, light.Color };
				++index;
			}
			return index;
		}

		return 0;
	}

}