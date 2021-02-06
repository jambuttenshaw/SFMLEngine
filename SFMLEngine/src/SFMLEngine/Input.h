#pragma once

#include <SFML/Graphics.hpp>

#include "Systems/CameraSystem.h"

namespace SFMLEngine {

	class Input
	{
	public:

		static void Init(sf::RenderWindow* window, std::shared_ptr<CameraSystem> cameraSystem);

		static bool IsKeyDown(sf::Keyboard::Key key);

		static bool IsMouseButtonDown(sf::Mouse::Button);

		static sf::Vector2i GetMouseScreenPos();
		static sf::Vector2f GetMouseWorldPos();

	private:
		static sf::RenderWindow* s_Window;
		static std::shared_ptr<CameraSystem> s_CameraSystem;

	};

}
