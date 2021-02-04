#pragma once

#include "../../ECS/Coordinator.h"
#include "../../ECS/Components.h"

namespace SFMLEngine {

	struct PointLightData
	{
		sf::Vector3f Position;
		float Intensity;
		float Range;
		sf::Color Color;

		PointLightData()
			: Position(), Intensity(0), Range(0), Color(sf::Color::White)
		{}
		PointLightData(const sf::Vector3f& pos, float intensity, float range, const sf::Color& color)
			: Position(pos), Intensity(intensity), Range(range), Color(color)
		{}
		PointLightData(const PointLightData& lightData)
			: Intensity(lightData.Intensity), Range(lightData.Range)
		{
			this->Position = lightData.Position;
			this->Color = lightData.Color;
		}
	};

	class PointLightSystem : public System
	{
	public:
		PointLightSystem() = default;
		~PointLightSystem() = default;

		void Init(Coordinator* coordinator)
		{
			m_Coordinator = coordinator;
		}

		void EntityAddedToSystem(Entity entity) override {};

		int GetLightingData(PointLightData* lightArray);

	private:
		Coordinator* m_Coordinator = nullptr;
	};

}
