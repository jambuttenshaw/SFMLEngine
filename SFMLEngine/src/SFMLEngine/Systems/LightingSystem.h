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

		LightData()
			: Position(), Intensity(0), Range(0), Color(sf::Color::White)
		{}
		LightData(const sf::Vector3f& pos, float intensity, float range, const sf::Color& color)
			: Position(pos), Intensity(intensity), Range(range), Color(color)
		{}
		LightData(const LightData& lightData)
			: Intensity(lightData.Intensity), Range(lightData.Range)
		{
			this->Position = lightData.Position;
			this->Color = lightData.Color;
		}
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

		int GetLightingData(LightData* lightArray);

	private:
		Coordinator* m_Coordinator = nullptr;
	};

}
