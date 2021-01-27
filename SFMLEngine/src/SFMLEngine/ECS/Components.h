#pragma once

#include <SFML/Graphics.hpp>


namespace SFMLEngine {

	struct Transform 
	{
		sf::Vector2f Position;
		float Rotation = 0.0f;
		sf::Vector2f Scale;
	};

	struct SpriteRenderer
	{
		sf::Sprite Sprite;
		sf::Texture Texture;

	};

}
