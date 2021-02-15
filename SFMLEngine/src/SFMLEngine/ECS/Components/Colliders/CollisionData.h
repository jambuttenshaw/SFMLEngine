#pragma once


#include <SFML/Graphics.hpp>


namespace SFMLEngine {

	struct CollisionData
	{
		bool Collided;
		sf::FloatRect Hitbox;
		sf::Vector2f Offset;
	};

}
