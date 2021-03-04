#include "Math.h"

namespace SFMLEngine {

	std::mt19937* Math::s_RandomNumberGenerator = nullptr;
	std::uniform_real_distribution<float> Math::s_NormamlizedFloatDistribution(0, 1);

	void Math::Init()
	{
		// initialise the random number generator with a random seed
		std::random_device rd;
		s_RandomNumberGenerator = new std::mt19937(rd());
	}
	void Math::Init(unsigned int seed)
	{
		s_RandomNumberGenerator = new std::mt19937(seed);
	}

	void Math::Shutdown()
	{
		delete s_RandomNumberGenerator;
	}


	// VECTOR 2
	sf::Vector2f Math::Max(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return sf::Vector2f(std::max(a.x, b.x), std::max(a.y, b.y));
	}
	sf::Vector2f Math::Min(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return sf::Vector2f(std::min(a.x, b.x), std::max(a.y, b.y));
	}
	float Math::Dot(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return a.x * b.x + a.y * b.y;
	}
	sf::Vector2f Math::Normalize(const sf::Vector2f& a)
	{
		auto mag = Magnitude(a);
		return mag == 0 ? a : a / mag;
	}
	sf::Vector2f Math::Clamp(const sf::Vector2f& value, const sf::Vector2f& min, const sf::Vector2f& max)
	{
		return Max(Min(value, max), min);
	}
	float Math::SquareMagnitude(const sf::Vector2f& a)
	{
		return Dot(a, a);
	}
	float Math::Magnitude(const sf::Vector2f& a)
	{
		return sqrtf(SquareMagnitude(a));
	}
	
	// VECTOR3
	sf::Vector3f Math::Max(const sf::Vector3f& a, const sf::Vector3f& b)
	{
		return sf::Vector3f(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
	}
	sf::Vector3f Math::Min(const sf::Vector3f& a, const sf::Vector3f& b)
	{
		return sf::Vector3f(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
	}
	float Math::Dot(const sf::Vector3f& a, const sf::Vector3f& b)
	{
		return a.x * b.x + a.y * b.y;
	}
	sf::Vector3f Math::Normalize(const sf::Vector3f& a)
	{
		return a / Magnitude(a);
	}
	sf::Vector3f Math::Clamp(const sf::Vector3f& value, const sf::Vector3f& min, const sf::Vector3f& max)
	{
		return Max(Min(value, max), min);
	}
	float Math::SquareMagnitude(const sf::Vector3f& a)
	{
		return Dot(a, a);
	}
	float Math::Magnitude(const sf::Vector3f& a)
	{
		return sqrtf(SquareMagnitude(a));
	}




	int Math::RandomInt(int max)
	{
		return RandomInt(0, max);
	}
	int Math::RandomInt(int min, int max)
	{
		std::uniform_int_distribution<int> dist(min, max);
		return dist(*s_RandomNumberGenerator);
	}

	float Math::RandomFloat(float max)
	{
		return RandomFloat(0, max);
	}
	float Math::RandomFloat(float min, float max)
	{
		std::uniform_real_distribution<float> dist(min, max);
		return dist(*s_RandomNumberGenerator);
	}
	float Math::RandomNormalizedFloat()
	{
		return s_NormamlizedFloatDistribution(*s_RandomNumberGenerator);
	}


}
