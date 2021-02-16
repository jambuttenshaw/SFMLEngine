#pragma once

#include <SFML/System.hpp>

#include <random>

namespace SFMLEngine {

	class Math
	{
	public:

		static void Init();
		static void Init(unsigned int seed);

		static void Shutdown();

		/*
		CONSTANTS
		*/

		const float RAD_TO_DEG = 57.2958f;
		const float DEG_TO_RAD = 0.0174533f;


		/*
		VECTOR MATH
		*/

		static sf::Vector2f Max(const sf::Vector2f& a, const sf::Vector2f& b);
		static sf::Vector2f Min(const sf::Vector2f& a, const sf::Vector2f& b);

		static float Dot(const sf::Vector2f& a, const sf::Vector2f& b);

		static const sf::Vector2f Clamp(const sf::Vector2f& value, const sf::Vector2f& min, const sf::Vector2f& max);

		static float SquareMagnitude(const sf::Vector2f& a);
		static float Magnitude(const sf::Vector2f& a);


		/*
		RANDOM NUMBERS
		*/
		static int RandomInt(int max);
		static int RandomInt(int min, int max);

		static float RandomFloat(int max);
		static float RandomFloat(int min, int max);
		static float RandomNormalizedFloat();

	private:
		static std::mt19937* s_RandomNumberGenerator;
		static std::uniform_real_distribution<float> s_NormamlizedFloatDistribution;

	};
}
