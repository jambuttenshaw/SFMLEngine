#pragma once

#include "../ECS/Coordinator.h"
#include "../ECS/Components.h"

namespace SFMLEngine {

	struct LightData
	{
	sf::Vector3f Position;
	float Intensity;
	float Range;
	sf::Color Color;
	};

	class LightingSystem : public System
	{
	public:
		LightingSystem() = default;
		~LightingSystem() = default;

		void Init(Coordinator* coordinator)
		{
			m_Coordinator = coordinator;
		}

		void EntityAddedToSystem(Entity entity) override {};

		int GetLightingData(LightData* lightArray)
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
		}

	private:
		Coordinator* m_Coordinator = nullptr;
	};

}
