#pragma once

#include <SFMLEngine.h>
using namespace SFMLEngine;


class PauseScript : public ScriptableEntity
{
public:

	static bool IsGamePaused() { return s_IsPaused; }

	void Update(float ts) override
	{
		if (Input::IsKeyPressed(sf::Keyboard::Escape))
		{
			AudioSystem::PlaySound("buttonClick");
			if (s_IsPaused)
			{
				Resume();
			}
			else
			{
				Pause();
			}
		}
	}

	void Pause();
	void Resume();

private:
	static bool s_IsPaused;
};

