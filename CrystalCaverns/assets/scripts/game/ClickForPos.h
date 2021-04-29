#pragma once

#include <SFMLEngine.h>
using namespace SFMLEngine;


class ClickForPos : public ScriptableEntity
{
public:
	void Start() override
	{
		m_Ground = &GetComponent<Tilemap>(GetEntitiesWithTag("Ground")[0]);
	}


	void Update(float ts) override
	{
		if (Input::IsMouseButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2f worldPos(Input::GetMouseWorldPos());
			LOG_INFO("{0}, {1}", worldPos.x, worldPos.y);
		}
		else if (Input::IsMouseButtonPressed(sf::Mouse::Right))
		{
			sf::Vector2f tilePos(m_Ground->TileToWorldCoordinates(m_Ground->WorldToTileCoordinates(Input::GetMouseWorldPos())));
			LOG_INFO("{0}, {1}", tilePos.x, tilePos.y);
		}
	}

private:
	Tilemap* m_Ground;
};
