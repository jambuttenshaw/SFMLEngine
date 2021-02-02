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

		t += 0.5f * ts;
		//lightPosition = sf::Vector3f(128, 128, 1);// +(200.0f * sf::Vector3f(cos(t), sin(t), 0));
		sf::Vector2i mouse = sf::Mouse::getPosition(*Application::GetApplicationHandle()->GetWindowHandle());
		lightPosition = sf::Vector3f(mouse.x, mouse.y, -5);
		m_Material->SetUniform("u_LightPos", sf::Glsl::Vec3(lightPosition));
		//m_EntityScript->SetPosition(sf::Vector2f(lightPosition.x, lightPosition.y));
	}

	//void SetEntityScript(EntityScript* script) { m_EntityScript = script; }

private:
	Material* m_Material;

	float t = 0;
	sf::Vector3f lightPosition;

	//EntityScript* m_EntityScript;
};
