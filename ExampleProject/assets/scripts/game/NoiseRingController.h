#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;


class NoiseRingController : public ScriptableEntity
{
public:
	void Start() override
	{
		m_Transform = &GetComponent<Transform>();
		m_Transform->SetScale({ 0, 0 });

		m_Sprite = &GetComponent<SpriteRenderer>();
	}
	

	void Update(float ts) override
	{
		m_Scale += m_ScaleIncreaseSpeed * ts;
		m_ScaleIncreaseSpeed += m_ScaleIncreaseSpeed * ts;
		

		sf::Vector2f scaledSize{ m_Scale * m_TextureSize };
		if (scaledSize.x > m_MaxScale)
		{
			Destroy();
		}

		float t = std::sqrt(std::max(0.0f, (1 - scaledSize.x / m_MaxScale)));
		float alpha = 255 * t;
		m_Sprite->SetColor({ 255, 255, 255, static_cast<sf::Uint8>(alpha) });
		

		m_Transform->SetScale({ m_Scale, m_Scale });
		m_Transform->SetPosition({ m_CentrePos - 0.5f * scaledSize });
	}

	void SetCentrePosition(const sf::Vector2f& centre)
	{
		m_CentrePos = centre;
	}

private:
	Transform* m_Transform;
	SpriteRenderer* m_Sprite;

	sf::Vector2f m_CentrePos;
	const sf::Vector2f m_TextureSize{ 64, 64 };

	float m_Scale = 0;
	float m_ScaleIncreaseSpeed = 6.0f;
	float m_MaxScale = 400.0f;
};
