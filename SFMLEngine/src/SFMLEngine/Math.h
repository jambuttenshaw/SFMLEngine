#pragma once

#include <SFML/System.hpp>

namespace SFMLEngine {

	sf::Vector2f Max(const sf::Vector2f& a, const sf::Vector2f& b);
	sf::Vector2f Min(const sf::Vector2f& a, const sf::Vector2f& b);

	float Dot(const sf::Vector2f& a, const sf::Vector2f& b);

	const sf::Vector2f Clamp(const sf::Vector2f& value, const sf::Vector2f& min, const sf::Vector2f& max);

	float SquareMagnitude(const sf::Vector2f& a);
	float Magnitude(const sf::Vector2f& a);
}
