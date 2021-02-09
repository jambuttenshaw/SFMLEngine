#pragma once

#include "../SFMLEngine.h"

using namespace SFMLEngine;

class ClickToDestroyTile : public ScriptableEntity
{
public:
	void Start() override
	{
		// do something when the game starts
		m_Tilemap = &GetComponent<Tilemap>();
	}

	void Update(Timestep ts) override
	{
		// do something every frame
		bool mouseDown = Input::IsMouseButtonDown(sf::Mouse::Left);
		if (mouseDown && !m_Click)
		{
			m_Click = true;
			m_Tilemap->RemoveTile(m_Tilemap->WorldToTileCoordinates(Input::GetMouseWorldPos()));
		}
		else if (!mouseDown && m_Click)
		{
			m_Click = false;
		}
	}

private:
	Tilemap* m_Tilemap = nullptr;
	bool m_Click = false;

};
