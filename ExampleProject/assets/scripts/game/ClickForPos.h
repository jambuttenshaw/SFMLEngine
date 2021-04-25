#pragma once

#include <SFMLEngine.h>
using namespace SFMLEngine;


class ClickForPos : public ScriptableEntity
{
public:

	void Update(float ts) override
	{
		if (Input::IsMouseButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2f worldPos(Input::GetMouseWorldPos());
			LOG_INFO("{0}, {1}", worldPos.x, worldPos.y);
		}
	}
};
