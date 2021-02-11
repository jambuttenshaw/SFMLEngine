#include "Math.h"

namespace SFMLEngine {

	sf::Vector2f Max(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return sf::Vector2f(std::max(a.x, b.x), std::max(a.y, b.y));
	}


	sf::Vector2f Min(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return sf::Vector2f(std::min(a.x, b.x), std::max(a.y, b.y));
	}


	float Dot(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return a.x * b.x + a.y * b.y;
	}


	const sf::Vector2f Clamp(const sf::Vector2f& value, const sf::Vector2f& min, const sf::Vector2f& max)
	{
		return Max(Min(value, max), min);
	}


	float SquareMagnitude(const sf::Vector2f& a)
	{
		return Dot(a, a);
	}


	float Magnitude(const sf::Vector2f& a)
	{
		return sqrtf(SquareMagnitude(a));
	}

}
