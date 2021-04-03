#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

class CrystalCollector : public ScriptableEntity
{
public:
	void Start() override
	{
		
	}

	void Update(Timestep ts) override
	{
		if (Input::IsKeyPressed(sf::Keyboard::Space))
		{
			if (m_CollidingWithCrystal)
			{
				m_CrystalMap->RemoveTile(m_CollidingCrystalPos);
			}
		}
	}

	void OnTriggerEnter(const Collision& collision) override
	{
		if (GetEntityTag(collision.Other) == "Crystals")
		{
			m_CollidingWithCrystal = true;
			m_CollidingCrystalPos = m_CrystalMap->WorldToTileCoordinates({ collision.OtherGlobalBounds.left, collision.OtherGlobalBounds.top });
		}
	}

	void OnTriggerExit(Entity other)
	{
		if (GetEntityTag(other) == "Crystals")
		{
			m_CollidingWithCrystal = false;
		}
	}

	void SetCrystalMap(Entity crystals)
	{
		m_CrystalMap = &GetComponent<Tilemap>(crystals);
	}

private:
	Tilemap* m_CrystalMap;

	bool m_CollidingWithCrystal = false;
	sf::Vector2i m_CollidingCrystalPos;
};
