#pragma once

#include "../SFMLEngine.h"

using namespace SFMLEngine;

class EntityScript : public ScriptableEntity
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
		
		t += ts;
		color = 127 * (1 + sin(t));
		m_Material->SetUniform("u_Color", sf::Glsl::Vec4(sf::Color(color, 255 - color, color, 255)));
	}

private:
	Material* m_Material;

	float t = 0;
	float color = 0;
};
