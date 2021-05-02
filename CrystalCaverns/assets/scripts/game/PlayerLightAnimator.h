#pragma once

#include <SFMLEngine.h>
using namespace SFMLEngine;


class PlayerLightAnimator : public ScriptableEntity
{
public:

	void Start() override
	{
		m_Light = &GetComponent<PointLight>(GetEntitiesWithTag("PlayerLight")[0]);
		m_InitialColor = m_Light->GetColor();
	}


	void Update(float ts) override
	{
		if (m_Animating)
		{
			m_AnimateProgress += ts;
			if (m_AnimateProgress >= m_AnimateTime)
			{
				m_Animating = false;
				m_Light->SetColor(m_InitialColor);
			}
			else
			{
				m_Light->SetColor(Math::ColorLerpComponents(m_TargetColor, m_InitialColor, m_AnimateProgress / m_AnimateTime));
			}
		}
	}


	void BeginAnimate(const sf::Color& color, float strength)
	{
		m_TargetColor = Math::ColorLerpComponents(m_InitialColor, color, Math::Clamp(strength, 0, 1));

		m_AnimateProgress = 0.0f;
		m_Animating = true;
	}

private:
	PointLight* m_Light = nullptr;

	sf::Color m_InitialColor;
	sf::Color m_TargetColor;

	bool m_Animating = false;
	float m_AnimateTime = 0.3f;
	float m_AnimateProgress = 0.0f;
};