#pragma once

#include <SFML/Graphics.hpp>


namespace SFMLEngine {

	struct Transform 
	{
		sf::Vector2f Position;
		float Rotation;
		sf::Vector2f Scale;
	};

}
