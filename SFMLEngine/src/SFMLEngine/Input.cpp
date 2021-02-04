#include "Input.h"


namespace SFMLEngine {

	sf::RenderWindow* Input::s_Window = nullptr;

	void Input::Init(sf::RenderWindow* window)
	{
		s_Window = window;
	}

	bool Input::IsKeyDown(sf::Keyboard::Key key)
	{
		return sf::Keyboard::isKeyPressed(key);
	}

	bool Input::IsMouseButtonDown(sf::Mouse::Button button)
	{
		return sf::Mouse::isButtonPressed(button);
	}

	const sf::Vector2i& Input::GetMouseScreenPos()
	{
		return sf::Mouse::getPosition(*s_Window);
	}

	const sf::Vector2f& Input::GetMouseWorldPos()
	{
		return s_Window->mapPixelToCoords(GetMouseScreenPos());
	}

}

