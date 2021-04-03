#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;


class MenuCameraController : public ScriptableEntity
{
public:
	void Start() override
	{
		m_Transform = &GetComponent<Transform>();
	}

	void Update(Timestep ts) override
	{
		m_Transform->Position = Math::Lerp(sf::Vector2f{0, 0}, Input::GetMouseWorldPos(), 0.05f);
	}


private:
	Transform* m_Transform = nullptr;
};
