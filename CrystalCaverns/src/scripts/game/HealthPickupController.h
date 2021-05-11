#pragma once

#include <SFMLEngine.h>
using namespace SFMLEngine;

/*
The health pickup controller animates the pickups,
and makes them disappear once theyve been collected
*/
class HealthPickupController : public ScriptableEntity
{
public:


	void Start() override
	{
		m_Transform = &GetComponent<Transform>();
		m_SpriteRenderer = &GetComponent<SpriteRenderer>();

		// store the initial y as our movement will always be relative to the initial position
		m_InitY = m_Transform->GetPosition().y;

		AudioSystem::LoadSound("HealthPickup", "assets/audio/healthPickup.ogg");
	}

	void Update(float ts) override
	{
		// should the pickup be fading out?
		if (m_Fading)
		{
			// increment timer
			m_FadeProgress += ts * m_FadeSpeed;
			if (m_FadeProgress >= 1)
			{
				// the heart has fully faded out
				// it should now be deleted
				m_FadeProgress = 1;

				// queue the entity for destruction at the beginning of the next frame
				Destroy();
			}
			else
			{
				// fade out the alpha channel
				// from 255 to 0
				// so the sprite smoothly disappears
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
				// stop anim progres from ever being bigger than it needs to be
				m_AnimProgress = 0.0f;
			}

			// the y position oscillates with a sine wave
			// scaled by the anim height
			float pos = m_AnimHeight * std::sin(m_AnimProgress);
			m_Transform->SetPosition({ m_Transform->GetPosition().x, m_InitY + pos });
		}
		
	}


	void CollectHeart()
	{
		// mark that the heart should begin fading out
		m_Fading = true;
		// position the pickup sound to the location of the heart before playing it
		AudioSystem::SetPosition("HealthPickup", m_Transform->GetPosition() + sf::Vector2f{ 16, 16 });
		AudioSystem::PlaySound("HealthPickup", true);


		// remove the collider so that the player doesn't interact with the heart after it has been collected
		RemoveComponent<BoxCollider>();
		RemoveComponent<ColliderInfo>();
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
