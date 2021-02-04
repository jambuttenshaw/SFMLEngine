#pragma once

#include "../SFMLEngine.h"

using namespace SFMLEngine;

class LightMovement : public ScriptableEntity
{
public:
	void Start() override
	{
		// do something when the game starts
		m_Material = GetComponent<SpriteRenderer>().GetMaterial();
	}

	void Update(Timestep ts) override
	{
		// do something every frame

		sf::Vector2f mouse = Input::GetMouseWorldPos();
		lightPosition = sf::Vector3f(mouse.x, mouse.y, -5.0f);

		m_Material->SetProperty("u_Lights[0].Position", lightPosition);
	}

private:
	Material* m_Material = nullptr;

	sf::Vector3f lightPosition;
};
