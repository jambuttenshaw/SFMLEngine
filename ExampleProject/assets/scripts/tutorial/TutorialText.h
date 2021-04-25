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
		m_Player = &GetComponent<Transform>(GetEntitiesWithTag("Player")[0]);
	}


	void Update(float ts) override
	{
		if (m_Fading)
		{
			m_FadeProgess += ts;
			if (m_FadeProgess >= 1)
			{
				Destroy();
			}
			else
			{
				m_Sprite->SetColor({ 255, 255, 255, sf::Uint8(Math::Lerp(255.0f, 0.0f, m_FadeProgess)) });
			}
		}
		else
		{
			if (!m_PlayerPassed)
			{
				if (fabsf(m_Transform->GetPosition().x - m_Player->GetPosition().x) < m_PlayerPassThreshold)
				{
					m_PlayerPassed = true;
				}
			}
			else
			{
				if (fabsf(m_Transform->GetPosition().x - m_Player->GetPosition().x) > m_DeleteAfterPassThreshold)
				{
					m_Fading = true;
				}
			}
		}
	}



	void Init(int textIndex, const sf::Vector2f& position)
	{
		AddComponent(Transform{ position });
		AddComponent(SpriteRenderer{
			Texture::Create("assets/textures/text/text" + std::to_string(textIndex) + ".png"),
			Material::Create("Basic"),
			3
			});
	}

private:
	Transform* m_Transform = nullptr;
	SpriteRenderer* m_Sprite = nullptr;

	Transform* m_Player = nullptr;
	bool m_PlayerPassed = false;

	bool m_Fading = false;
	float m_FadeProgess = 0.0f;


	const float m_PlayerPassThreshold = 20.0f;
	const float m_DeleteAfterPassThreshold = 150.f;

};
