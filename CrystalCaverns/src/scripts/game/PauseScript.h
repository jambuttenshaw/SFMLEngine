#pragma once

#include <SFMLEngine.h>
using namespace SFMLEngine;


class PauseScript : public ScriptableEntity
{
public:

	// find out if the game is paused
	// this is static as any script should be able to know if the game is paused or not
	// pausing is a global state that effects everything
	static bool IsGamePaused() { return s_IsPaused; }

	void Update(float ts) override
	{
		// look out for when the escape key is pressed
		if (Input::IsKeyPressed(sf::Keyboard::Escape))
		{
			if (s_IsPaused)
			{
				Resume();
			}
			else
			{
				Pause();
			}
			// play the click sound AFTERWARD because pausing the game pauses all sounds
			AudioSystem::PlaySound("buttonClick");
		}
	}

	void Pause();
	void Resume();

private:
	static bool s_IsPaused;
};

