#pragma once

#include <SFMLEngine.h>
using namespace SFMLEngine;

#include "game/PlayerController.h"


class TutorialText : public ScriptableEntity
{
public:

	void Start() override
	{
		m_Transform = &GetComponent<Transform>();
		m_Sprite = &GetComponent<SpriteRenderer>();
		m_Player = &GetNativeScript<PlayerController>(GetEntitiesWithTag("Player")[0]);

		// the centre is used as a comparison point against the player
		m_Centre = m_Transform->GetPosition() + 0.5f * m_Sprite->GetDimensions();
	}


	void Update(float ts) override
	{
		// if the text is fading out
		if (m_Fading)
		{
			// increment the timer
			m_FadeProgess += ts;
			if (m_FadeProgess >= 1)
			{
				// the fade has completed
				// this entity can be destroyed
				Destroy();
			}
			else
			{
				// slowly reduce the alpha channel of the text
				m_Sprite->SetColor({ 255, 255, 255, sf::Uint8(Math::Lerp(255.0f, 0.0f, m_FadeProgess)) });
			}
		}
		else
		{
			// are we still waiting for the player to pass this text?
			if (!m_PlayerPassed)
			{
				// check the distance to the player
				// only consider the horizontal distance
				if (fabsf(m_Centre.x - m_Player->GetCentre().x) < m_PlayerPassThreshold)
				{
					// the player got close enough to the text to say that it has passed
					m_PlayerPassed = true;
				}
			}
			else
			{
				// now wait for the player to move far enough away that we can fade out this text
				if (Math::SquareMagnitude(m_Centre - m_Player->GetCentre()) > m_DeleteAfterPassThreshold * m_DeleteAfterPassThreshold)
				{
					m_Fading = true;
				}
			}
		}
	}



	void Init(int textIndex, const sf::Vector2f& position)
	{
		// add a transform to this entity
		AddComponent(Transform{ position });
		// load the text sprite
		// text is just rendered with the basic shader
		AddComponent(SpriteRenderer{
			Texture::Create("assets/textures/text/text" + std::to_string(textIndex) + ".png"),
			Material::Create("Basic"),
			3
			});
	}

private:
	Transform* m_Transform = nullptr;
	SpriteRenderer* m_Sprite = nullptr;

	sf::Vector2f m_Centre;

	PlayerController* m_Player = nullptr;
	bool m_PlayerPassed = false;

	bool m_Fading = false;
	float m_FadeProgess = 0.0f;

	// defines how far the player should be from the text to trigger certain events
	const float m_PlayerPassThreshold = 20.0f;
	const float m_DeleteAfterPassThreshold = 150.0f;

};
