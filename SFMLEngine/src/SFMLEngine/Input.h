#pragma once

#include <SFML/Graphics.hpp>

namespace SFMLEngine {

	class Input
	{
	public:

		static void Init(sf::RenderWindow* window);

		static bool IsKeyDown(sf::Keyboard::Key key);

		static bool IsMouseButtonDown(sf::Mouse::Button);

		static sf::Vector2i GetMouseScreenPos();
		static sf::Vector2f GetMouseWorldPos();

	private:

		static sf::RenderWindow* s_Window;

	};

}
