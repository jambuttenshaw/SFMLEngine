#pragma once

#include <SFMLEngine.h>

#include "Constants.h"

using namespace SFMLEngine;


class NoiseRingController : public ScriptableEntity
{
public:
	void Start() override
	{
		m_Transform = &GetComponent<Transform>();
		// start scaled all the way down
		m_Transform->SetScale({ 0, 0 });

		m_Sprite = &GetComponent<SpriteRenderer>();
	}
	

	void Update(float ts) override
	{
		// increase the scale
		m_Scale += m_ScaleIncreaseSpeed * ts;
		// to make the area increase at a constant rate,
		// m_Scale needs to increase quadratically
		// so increase the increase speed by the increase speed
		m_ScaleIncreaseSpeed += m_ScaleIncreaseSpeed * ts;
		

		sf::Vector2f scaledSize{ m_Scale * m_TextureSize };
		// the ring is now so big we can just destroy it
		if (scaledSize.x > WOLF_WAKE_DISTANCE)
		{
			Destroy();
		}

		// fade out the opacity of the sprite
		// clamp it to 0 before taking sqrt to avoid bad stuff
		float t = std::sqrt(std::max(0.0f, (1 - scaledSize.x / WOLF_WAKE_DISTANCE)));
		float alpha = 255 * t;
		m_Sprite->SetColor({ 255, 255, 255, static_cast<sf::Uint8>(alpha) });
		
		// update the transforms scale and position
		m_Transform->SetScale({ m_Scale, m_Scale });
		// always keep it centred on the same position
		m_Transform->SetPosition({ m_CentrePos - 0.5f * scaledSize });
	}

	void SetCentrePosition(const sf::Vector2f& centre)
	{
		// set the position when its created
		m_CentrePos = centre;
	}

private:
	Transform* m_Transform = nullptr;
	SpriteRenderer* m_Sprite = nullptr;

	sf::Vector2f m_CentrePos;
	// we need to know the texture size so we can work out how big the scaled version is
	const sf::Vector2f m_TextureSize{ 64, 64 };

	float m_Scale = 0;
	float m_ScaleIncreaseSpeed = 6.0f;
};
