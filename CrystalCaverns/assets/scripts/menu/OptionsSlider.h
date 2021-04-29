#pragma once

#include <SFMLEngine.h>
using namespace SFMLEngine;

#include "KnobController.h"


class OptionsSlider : public ScriptableEntity
{
public:

	void Start() override
	{
		// create the options knob
		sf::Vector2f pos = GetComponent<Transform>().GetPosition();

		// 168, 32
		// 280, 32
		m_Knob = CreateEntity();
		AddComponent(m_Knob, Transform{ pos + sf::Vector2f{267.5f, 19.5f} });
		AddComponent(m_Knob, SpriteRenderer{
				Texture::Create("assets/textures/buttons/knob.png"),
				Material::Create("Lit"),
				1,
				Texture::Create("assets/textures/buttons/knob_n.png")
			});
		m_KnobController = &AddNativeScript<KnobController>(m_Knob);
		m_KnobController->SetBoundaries(pos.x + 155.5f, pos.x + 267.5f);
	}

	void Update(float ts) override
	{
		float old = m_Value;
		m_Value = Math::Lerp(m_MinValue, m_MaxValue, m_KnobController->GetPercent());

		if (m_Value != old)
			m_OnSliderChange(m_Value);
	}


	void SetSliderFunction(const std::function<void(float)>& onChange, float minVal, float maxVal)
	{
		m_OnSliderChange = onChange;
		m_MinValue = minVal;
		m_MaxValue = maxVal;
	}


private:
	Entity m_Knob;
	KnobController* m_KnobController;

	float m_Value = 0;
	std::function<void(float)> m_OnSliderChange;
	float m_MinValue = 0;
	float m_MaxValue = 0;

};