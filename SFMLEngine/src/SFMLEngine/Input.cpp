#include "Input.h"

#include "Log.h"

namespace SFMLEngine {

	sf::RenderWindow* Input::s_Window = nullptr;
	std::shared_ptr<CameraSystem> Input::s_CameraSystem;

	void Input::Init(sf::RenderWindow* window, std::shared_ptr<CameraSystem> cameraSystem)
	{
		s_Window = window;
		s_CameraSystem = cameraSystem;
	}

	bool Input::IsKeyDown(sf::Keyboard::Key key)
	{
		return sf::Keyboard::isKeyPressed(key);
	}

	bool Input::IsMouseButtonDown(sf::Mouse::Button button)
	{
		return sf::Mouse::isButtonPressed(button);
	}

	sf::Vector2i Input::GetMouseScreenPos()
	{
		return sf::Mouse::getPosition(*s_Window);
	}

	sf::Vector2f Input::GetMouseWorldPos()
	{
		return s_Window->mapPixelToCoords(GetMouseScreenPos(), s_CameraSystem->GetMainCameraView());
	}

}

