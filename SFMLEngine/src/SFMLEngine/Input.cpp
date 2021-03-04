#include "Input.h"

#include "Core.h"

namespace SFMLEngine {

	sf::RenderWindow* Input::s_Window = nullptr;
	std::shared_ptr<CameraSystem> Input::s_CameraSystem;

	sf::Vector2f Input::s_MouseDelta;
	sf::Vector2f Input::s_OldMousePos;
	int Input::s_WheelDelta = 0;

	bool Input::s_WindowFocused = true;

	std::set<int> Input::s_KeysPressed;
	std::set<int> Input::s_MouseButtonsPressed;

	void Input::Init(sf::RenderWindow* window, std::shared_ptr<CameraSystem> cameraSystem)
	{
		ZoneScoped;

		s_Window = window;
		s_CameraSystem = cameraSystem;
	}

	bool Input::IsKeyDown(sf::Keyboard::Key key)
	{
		return sf::Keyboard::isKeyPressed(key) && s_WindowFocused;
	}

	bool Input::IsKeyPressed(sf::Keyboard::Key key)
	{
		// if key is inside the set then it has been pressed this frame
		return s_KeysPressed.find(static_cast<int>(key)) != s_KeysPressed.end();
	}

	bool Input::IsMouseButtonDown(sf::Mouse::Button button)
	{
		return sf::Mouse::isButtonPressed(button) && s_WindowFocused;
	}

	bool Input::IsMouseButtonPressed(sf::Mouse::Button button)
	{
		return s_MouseButtonsPressed.find(static_cast<int>(button)) != s_MouseButtonsPressed.end();
	}

	sf::Vector2i Input::GetMouseScreenPos()
	{
		return sf::Mouse::getPosition(*s_Window);
	}

	sf::Vector2f Input::GetMouseWorldPos()
	{
		return s_Window->mapPixelToCoords(GetMouseScreenPos(), s_CameraSystem->GetMainCameraView());
	}

	void Input::Reset()
	{ 
		ZoneScoped;

		s_MouseDelta = sf::Vector2f(0, 0);
		s_WheelDelta = 0; 

		s_KeysPressed.clear();
		s_MouseButtonsPressed.clear();
	}
}

