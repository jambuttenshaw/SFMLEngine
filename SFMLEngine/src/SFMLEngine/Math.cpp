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


	// FLOAT
	float Math::Clamp(float value, float min, float max)
	{
		return std::max(std::min(value, max), min);
	}


	// VECTOR 2
	sf::Vector2f Math::Max(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return sf::Vector2f(std::max(a.x, b.x), std::max(a.y, b.y));
	}
	sf::Vector2f Math::Min(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return sf::Vector2f(std::min(a.x, b.x), std::min(a.y, b.y));
	}
	sf::Vector2f Math::Abs(const sf::Vector2f& a)
	{
		return sf::Vector2f(fabsf(a.x), fabsf(a.y));
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
	Math::Direction Math::GetDirection(const sf::Vector2f& a)
	{
		float dirX = Dot(Normalize(a), UnitVector::Right);
		float dirY = Dot(Normalize(a), UnitVector::Down);
		if (fabsf(dirY) >= fabsf(dirX))
		{
			return dirY > 0 ? Direction::Down : Direction::Up;
		}
		else
		{
			return dirX > 0 ? Direction::Right : Direction::Left;
		}
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
	sf::Vector3f Math::Abs(const sf::Vector3f& a)
	{
		return sf::Vector3f(fabsf(a.x), fabsf(a.y), fabsf(a.z));
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



	sf::FloatRect Math::Intersection(const sf::FloatRect& a, const sf::FloatRect& b)
	{
		return {
			std::max(a.left, b.left),
			std::max(a.top, b.top),
			std::max(0.0f, std::min(a.left + a.width,  b.left + b.width) - std::max(a.left, b.left)),
			std::max(0.0f, std::min(a.top + a.height,  b.top + b.height) - std::max(a.top,  b.top))
		};
	}
	sf::Vector2f Math::Centroid(const sf::FloatRect& a)
	{
		return { a.left + 0.5f * a.width, a.top + 0.5f * a.height };
	}
	std::pair<sf::Vector2f, sf::Vector2f> Math::GetCorners(const sf::FloatRect& a)
	{
		return std::make_pair(
			sf::Vector2f{ a.left, a.top },
			sf::Vector2f{ a.left + a.width, a.top + a.height }
		);
	}


	sf::Color Math::ColorLerpComponents(const sf::Color& a, const sf::Color& b, float t)
	{
		sf::Vector3f aComponents{ (float)a.r, (float)a.g, (float)a.b };
		sf::Vector3f bComponents{ (float)b.r, (float)b.g, (float)b.b };
		sf::Vector3<sf::Uint8> lerped{Math::Lerp(aComponents, bComponents, t)};
		return sf::Color{ lerped.x, lerped.y, lerped.z, Math::Lerp(a.a, b.a, t) };
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

	sf::Vector2f Math::RandomUnitVector()
	{
		float angle = RandomFloat(0, PI);
		return sf::Vector2f{
			cos(angle),
			sin(angle)
		};
	}


}
