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
		m_Transform = &GetComponent<Transform>();
	}

	void Update(Timestep ts) override
	{
		// do something every frame
		
		t += ts;
		color = 127 * (1 + sin(t));
		m_Material->SetUniform("u_Color", sf::Glsl::Vec4(sf::Color(color, 255 - color, color, 255)));
	}

	void SetPosition(sf::Vector2f pos)
	{
		m_Transform->Position = pos;
	}

private:
	Material* m_Material;
	Transform* m_Transform;

	float t = 0;
	float color = 0;
};
