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
		static bool IsKeyPressed(sf::Keyboard::Key key);

		static bool IsMouseButtonDown(sf::Mouse::Button button);
		static bool IsMouseButtonPressed(sf::Mouse::Button button);

		static sf::Vector2i GetMouseScreenPos();
		static sf::Vector2f GetMouseWorldPos();

		static const sf::Vector2f& GetMouseDelta() { return s_WindowFocused ? s_MouseDelta : sf::Vector2f(); }
		static int GetMouseWheelDelta() { return s_WindowFocused ? s_WheelDelta : 0; }

		static void SetFocused(bool focused) { s_WindowFocused = focused; }

	private:

		static void Reset();
		static void SetMouseDelta(const sf::Vector2f& newPos) { s_MouseDelta = s_OldMousePos - newPos; s_OldMousePos = newPos; }
		static void SetWheelDelta(int delta) { s_WheelDelta = delta; }
		static void SetKeyPressed(sf::Keyboard::Key key)
		{ 
			int keycode = static_cast<int>(key);
			if (s_WaitingForRelease.find(keycode) == s_WaitingForRelease.end())
			{
				s_KeysPressed.insert(keycode);
				s_WaitingForRelease.insert(keycode);
			}
		}
		static void SetMousePressed(sf::Mouse::Button button) { s_MouseButtonsPressed.insert(static_cast<int>(button)); }
		static void SetKeyUp(sf::Keyboard::Key key) { s_WaitingForRelease.erase(static_cast<int>(key)); }

	private:
		static sf::RenderWindow* s_Window;
		static std::shared_ptr<CameraSystem> s_CameraSystem;

		static sf::Vector2f s_MouseDelta;
		static sf::Vector2f s_OldMousePos;
		static int s_WheelDelta;

		static bool s_WindowFocused;

		// all of the keys pressed this frame
		static std::set<int> s_KeysPressed;
		static std::set<int> s_MouseButtonsPressed;

		static std::set<int> s_WaitingForRelease;

	};

}
