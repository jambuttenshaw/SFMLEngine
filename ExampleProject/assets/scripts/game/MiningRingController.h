#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;


class MiningRingController : public ScriptableEntity
{
public:
	void Start() override
	{
		m_Transform = &GetComponent<Transform>();
		m_Transform->Scale = { 0, 0 };
	}

	
	void Update(Timestep ts) override
	{
		m_Scale += m_ScaleIncreaseSpeed * ts;
		m_ScaleIncreaseSpeed += m_ScaleIncreaseSpeed * ts;
		

		sf::Vector2f scaledSize{ m_Scale * m_TextureSize };
		if (scaledSize.x > m_MaxScale)
		{
			Destroy();
		}


		m_Transform->Scale = { m_Scale, m_Scale };
		m_Transform->Position = m_CentrePos - 0.5f * scaledSize;
	}

	void SetCentrePosition(const sf::Vector2f& centre)
	{
		m_CentrePos = centre;
	}

private:
	Transform* m_Transform;

	sf::Vector2f m_CentrePos;
	const sf::Vector2f m_TextureSize{ 64, 64 };

	float m_Scale = 0;
	float m_ScaleIncreaseSpeed = 6.0f;
	float m_MaxScale = 300.0f;
};
