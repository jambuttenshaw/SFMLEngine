#pragma once

#include <SFML/System.hpp>

namespace SFMLEngine {

	sf::Vector2f Max(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return sf::Vector2f(std::max(a.x, b.x), std::max(a.y, b.y));
	}

}
