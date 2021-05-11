#pragma once

#include <SFMLEngine.h>
#include "Constants.h"
#include "WolfController.h"


using namespace SFMLEngine;

class WolfManager : public ScriptableEntity
{
public:
	void OnSceneLoaded() override
	{
		// get fresh wolf data every time a new scene is loaded
		// as the wolves in existence will have changed
		m_WolfControllers.clear();

		// get all of the wolves
		std::vector<Entity> wolves = GetEntitiesWithTag("Wolf");
		for (auto const& wolf : wolves)
		{
			// and get a pointer to their wolf controller
			m_WolfControllers.push_back(&GetNativeScript<WolfController>(wolf));
		}
	}

	void AwakenWolves(const sf::Vector2f& pos)
	{
		// iterate through all of the wolves
		for (auto& wolfController : m_WolfControllers)
		{
			// if they are close enough to the position the wake event occurred, then wake them up
			if (Math::SquareMagnitude(wolfController->GetCentre() - pos) < WOLF_WAKE_DISTANCE * WOLF_WAKE_DISTANCE)
			{
				wolfController->Wake();
			}
		}
	}


	void Update(float ts) override
	{
		// reset them to not stalling initially
		for (auto& wolf : m_WolfControllers)
		{
			wolf->SetStalling(false);
		}

		// compare every wolf to each other to make sure theyre not too close together
		for (auto& wolf : m_WolfControllers)
		{
			if (wolf->GetState() != WolfController::WolfState::Follow) continue;

			// if this wolf is really close to another wolf, we dont want them to run together right next to each other
			// we need to make one wolf wait and the other(s) run on ahead to stop them from bunching up

			// to achieve this, some wolf priority needs to be establish
			// since all wolves are created ALMOST equal, we can resort to their Entity ID's to prioritise them

			for (auto& other : m_WolfControllers)
			{
				// make sure it isnt the same wolf, and that it is in the correct state
				if (other == wolf || other->GetState() != WolfController::WolfState::Follow) continue;

				if (Math::SquareMagnitude(other->GetCentre() - wolf->GetCentre()) < m_WolfSeperationRadius * m_WolfSeperationRadius)
				{
					// wolves with a higher entity handle are allowed to keep on moving and the other ones are made to wait
					bool wolf1Stall = wolf->GetEntityHandle() < other->GetEntityHandle();
					wolf->SetStalling(wolf1Stall);
					other->SetStalling(!wolf1Stall);
				}
			}
		}
	}

private:
	// wolves chasing the player should be at least 1 tile apart at all times
	float m_WolfSeperationRadius = 32;

	std::vector<WolfController*> m_WolfControllers;

};
