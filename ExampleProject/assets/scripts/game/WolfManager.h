#pragma once

#include <SFMLEngine.h>

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
		m_WolfControllers.clear();

		std::vector<Entity> wolves = GetEntitiesWithTag("Wolf");
		for (auto const& wolf : wolves)
		{
			m_WolfTransforms.push_back(&GetComponent<Transform>(wolf));
			m_WolfControllers.push_back(&GetNativeScript<WolfController>(wolf));
		}
	}

	void AwakenWolves(const sf::Vector2f& pos)
	{
		for (size_t i = 0; i < m_WolfTransforms.size(); i++)
		{
			if (Math::SquareMagnitude(m_WolfTransforms[i]->GetPosition() - pos) < m_WolfTriggerRadius * m_WolfTriggerRadius)
			{
				m_WolfControllers[i]->Wake();
			}
		}
	}

private:
	float m_WolfTriggerRadius = 300;

	std::vector<Transform*> m_WolfTransforms;
	std::vector<WolfController*> m_WolfControllers;

};
