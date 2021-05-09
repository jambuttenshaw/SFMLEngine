#pragma once

#include <SFMLEngine.h>
using namespace SFMLEngine;


class HealthPickupController : public ScriptableEntity
{
public:


	void Start() override
	{
		m_Transform = &GetComponent<Transform>();
		m_SpriteRenderer = &GetComponent<SpriteRenderer>();

		m_InitY = m_Transform->GetPosition().y;

		AudioSystem::LoadSound("HealthPickup", "assets/audio/healthPickup.ogg");
	}

	void Update(float ts) override
	{
		if (m_Fading)
		{
			m_FadeProgress += ts * m_FadeSpeed;
			if (m_FadeProgress >= 1)
			{
				// the heart has fully faded out
				// it should now be deleted
				m_FadeProgress = 1;

				Destroy();
			}
			else
			{
				m_SpriteRenderer->SetColor({ 255, 255, 255, sf::Uint8((1 - m_FadeProgress) * 255) });
			}


			// when the heart is fading out we just want it to float upwards instead of oscillating up and down
			m_Transform->Translate({ 0, -(m_FadeSpeed * m_AnimSpeed * ts) });
		}
		else
		{
			// the heart should do a wiggle up and down to show its alive
			m_AnimProgress += m_AnimSpeed * ts;
			if (m_AnimProgress > 2 * Math::PI)
			{
				m_AnimProgress = 0.0f;
			}

			float pos = m_AnimHeight * std::sin(m_AnimProgress);
			m_Transform->SetPosition({ m_Transform->GetPosition().x, m_InitY + pos });
		}
		
	}


	void CollectHeart()
	{
		// mark that the heart should begin fading out
		m_Fading = true;
		AudioSystem::SetPosition("HealthPickup", m_Transform->GetPosition() + sf::Vector2f{ 16, 16 });
		AudioSystem::PlaySound("HealthPickup", true);
	}


private:
	Transform* m_Transform = nullptr;
	SpriteRenderer* m_SpriteRenderer = nullptr;

	// modify these
	float m_AnimSpeed = 3.0f;
	float m_AnimHeight = 4.0f;
	float m_FadeSpeed = 2.0f;

	// dont modify these
	float m_AnimProgress = 0.0f;
	float m_InitY = 0.0f;

	bool m_Fading = false;
	float m_FadeProgress = 0.0f;
};
