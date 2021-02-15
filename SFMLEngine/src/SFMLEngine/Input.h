#pragma once

#include <SFML/Graphics.hpp>

#include "Systems/CameraSystem.h"

namespace SFMLEngine {

	class Input
	{
	public:
		friend class Application;

		static void Init(sf::RenderWindow* window, std::shared_ptr<CameraSystem> cameraSystem);

		static bool IsKeyDown(sf::Keyboard::Key key);

		static bool IsMouseButtonDown(sf::Mouse::Button);

		static sf::Vector2i GetMouseScreenPos();
		static sf::Vector2f GetMouseWorldPos();

		static const sf::Vector2f& GetMouseDelta() { return s_WindowFocused ? s_MouseDelta : sf::Vector2f(); }
		static float GetMouseWheelDelta() { return s_WindowFocused ? s_WheelDelta : 0; }

		static void SetFocused(bool focused) { s_WindowFocused = focused; }

	private:

		static void ResetDeltas() { ZoneScoped; s_MouseDelta = sf::Vector2f(0, 0); s_WheelDelta = 0; }
		static void SetMouseDelta(const sf::Vector2f& newPos) { s_MouseDelta = s_OldMousePos - newPos; s_OldMousePos = newPos; }
		static void SetWheelDelta(float delta) { s_WheelDelta = delta; }

	private:
		static sf::RenderWindow* s_Window;
		static std::shared_ptr<CameraSystem> s_CameraSystem;

		static sf::Vector2f s_MouseDelta;
		static sf::Vector2f s_OldMousePos;
		static float s_WheelDelta;

		static bool s_WindowFocused;

	};

}
