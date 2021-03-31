#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;


class MainScene;


class MainMenuManager : public ScriptableEntity
{
public:
	void Start() override
	{

	}

	void Update(Timestep ts) override
	{
		if (Input::IsKeyPressed(sf::Keyboard::Enter))
			Application::GetApplicationHandle()->LoadScene<MainScene>();
	}


private:
	
};
