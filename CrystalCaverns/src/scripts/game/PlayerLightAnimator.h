#pragma once

#include <SFMLEngine.h>
using namespace SFMLEngine;


/*
PlayerLightAnimator controls the flashes of light that are produced
*/
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
		// if the light is currently being animated
		if (m_Animating)
		{
			// increment the progress
			m_AnimateProgress += ts;
			if (m_AnimateProgress >= m_AnimateTime)
			{
				// we have finished the animation, revert to the initial colour
				m_Animating = false;
				m_Light->SetColor(m_InitialColor);
			}
			else
			{
				// interpolate from the target back toward the inital colour
				m_Light->SetColor(Math::ColorLerpComponents(m_TargetColor, m_InitialColor, m_AnimateProgress / m_AnimateTime));
			}
		}
	}


	void BeginAnimate(const sf::Color& color, float strength)
	{
		// the target colour is 'strength' way between the specified colour and the initial one
		// so that the first few frames of the light animation arent too weird
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