#pragma once


#include <SFML/Graphics.hpp>


namespace SFMLEngine {

	struct CollisionData
	{
		bool Collided;
		sf::FloatRect Hitbox;
	};

}
