#pragma once

#include <SFMLEngine.h>
#include "Constants.h"
#include "WolfController.h"


using namespace SFMLEngine;

class WolfManager : public ScriptableEntity
{
public:
	// void Start() override;
	void OnSceneLoaded() override
	{
		// get fresh wolf data every time a new scene is loaded
		m_WolfControllers.clear();

		std::vector<Entity> wolves = GetEntitiesWithTag("Wolf");
		for (auto const& wolf : wolves)
		{
			m_WolfControllers.push_back(&GetNativeScript<WolfController>(wolf));
		}
	}

	void AwakenWolves(const sf::Vector2f& pos)
	{
		for (size_t i = 0; i < m_WolfControllers.size(); i++)
		{
			if (Math::SquareMagnitude(m_WolfControllers[i]->GetCentre() - pos) < WOLF_WAKE_DISTANCE * WOLF_WAKE_DISTANCE)
			{
				m_WolfControllers[i]->Wake();
			}
		}
	}


	void Update(float ts) override
	{
		for (auto& wolf : m_WolfControllers)
		{
			wolf->SetStalling(false);
		}

		// compare every wolf to each other to make sure theyre not too close together
		for (auto& wolf : m_WolfControllers)
		{

			// if this wolf is really close to another wolf, we dont want them to run together right next to each other
			// we need to make one wolf wait and the other(s) run on ahead to stop them from bunching up

			// to achieve this, some wolf priority needs to be establish
			// since all wolves are created ALMOST equal, we can resort to their Entity ID's to prioritise them

			for (auto& other : m_WolfControllers)
			{
				if (other == wolf || other->GetState() != WolfController::WolfState::Follow) continue;

				if (Math::SquareMagnitude(other->GetCentre() - wolf->GetCentre()) < m_WolfSeperationRadius * m_WolfSeperationRadius)
				{
					bool wolf1Stall = wolf->GetEntityHandle() < other->GetEntityHandle();
					wolf->SetStalling(wolf1Stall);
					other->SetStalling(!wolf1Stall);
				}
			}
		}
	}

private:
	float m_WolfSeperationRadius = 32;

	std::vector<WolfController*> m_WolfControllers;

};
