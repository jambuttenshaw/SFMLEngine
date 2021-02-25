#pragma once

#include <SFML/System.hpp>
#include "SFMLEngine/Core.h"
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

		static inline const float RAD_TO_DEG = 57.2958f;
		static inline const float DEG_TO_RAD = 0.0174533f;

		static inline const float PI = 3.1415926f;


		/*
		VECTOR MATH
		*/

		static sf::Vector2f Max(const sf::Vector2f& a, const sf::Vector2f& b);
		static sf::Vector3f Max(const sf::Vector3f& a, const sf::Vector3f& b);
		static sf::Vector2f Min(const sf::Vector2f& a, const sf::Vector2f& b);
		static sf::Vector3f Min(const sf::Vector3f& a, const sf::Vector3f& b);

		static float Dot(const sf::Vector2f& a, const sf::Vector2f& b);
		static float Dot(const sf::Vector3f& a, const sf::Vector3f& b);

		static sf::Vector2f Normalize(const sf::Vector2f& a);
		static sf::Vector3f Normalize(const sf::Vector3f& a);

		static sf::Vector2f Clamp(const sf::Vector2f& value, const sf::Vector2f& min, const sf::Vector2f& max);
		static sf::Vector3f Clamp(const sf::Vector3f& value, const sf::Vector3f& min, const sf::Vector3f& max);

		static float SquareMagnitude(const sf::Vector2f& a);
		static float SquareMagnitude(const sf::Vector3f& a);
		static float Magnitude(const sf::Vector2f& a);
		static float Magnitude(const sf::Vector3f& a);

		template<typename T>
		static T Lerp(const T& a, const T& b, float t) { return (1 - t) * a + t * b; }


		/*
		RANDOM NUMBERS
		*/
		static int RandomInt(int max);
		static int RandomInt(int min, int max);

		static float RandomFloat(float max);
		static float RandomFloat(float min, float max);
		static float RandomNormalizedFloat();

	private:
		static std::mt19937* s_RandomNumberGenerator;
		static std::uniform_real_distribution<float> s_NormamlizedFloatDistribution;

	};
}
