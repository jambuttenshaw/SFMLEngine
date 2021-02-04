#pragma once

#include "LightSystem.h"


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

	class DirectionalLightSystem : public LightSystem
	{
	public:
		DirectionalLightSystem() = default;
		~DirectionalLightSystem() = default;

		int GetLightingData(DirectionalLightData* lightArray);

	};

}
