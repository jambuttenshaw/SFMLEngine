#pragma once

#include "../../ECS/Coordinator.h"
#include "../../ECS/Components.h"


namespace SFMLEngine {

	struct DirectionalLightData
	{
		sf::Vector3f Direction;
		float Intensity;
		sf::Color Color;

		DirectionalLightData()
			: Direction(sf::Vector3f(0, 0, 1)), Intensity(0), Color(sf::Color::White)
		{}
		DirectionalLightData(const sf::Vector3f& dir, float intensity, const sf::Color& color)
			: Direction(dir), Intensity(intensity), Color(color)
		{}
		DirectionalLightData(const DirectionalLightData& lightData)
			: Intensity(lightData.Intensity)
		{
			this->Direction = lightData.Direction;
			this->Color = lightData.Color;
		}
	};

	class DirectionalLightSystem : public System
	{
	public:
		DirectionalLightSystem() = default;
		~DirectionalLightSystem() = default;

		void Init(Coordinator* coordinator)
		{
			m_Coordinator = coordinator;
		}

		void EntityAddedToSystem(Entity entity) override {};

		int GetLightingData(DirectionalLightData* lightArray);

	private:
		Coordinator* m_Coordinator = nullptr;
	};

}
