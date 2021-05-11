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

		// create a knob on the slider that controls the value of the control parameter
		m_Knob = CreateEntity();
		// place it at the max pos to start with
		AddComponent(m_Knob, Transform{ pos + sf::Vector2f{267.5f, 19.5f} });
		// load the texture
		AddComponent(m_Knob, SpriteRenderer{
				Texture::Create("assets/textures/buttons/knob.png"),
				Material::Create("Lit"),
				1,
				Texture::Create("assets/textures/buttons/knob_n.png")
			});
		// add the knob controller script
		m_KnobController = &AddNativeScript<KnobController>(m_Knob);
		// specify where the boundaries of the slider are
		m_KnobController->SetBoundaries(pos.x + 155.5f, pos.x + 267.5f);
	}

	void Update(float ts) override
	{
		// keep hold of the old value
		float old = m_Value;
		// update the current value
		m_Value = Math::Lerp(m_MinValue, m_MaxValue, m_KnobController->GetPercent());

		// if the value has changed since last frame
		if (m_Value != old)
			// call the control function
			m_OnSliderChange(m_Value);
	}


	void SetSliderFunction(const std::function<void(float)>& onChange, float minVal, float maxVal)
	{
		// set what happens when the slider is adjusted
		m_OnSliderChange = onChange;
		m_MinValue = minVal;
		m_MaxValue = maxVal;
	}


private:
	Entity m_Knob = INVALID_ENTITY_ID;
	KnobController* m_KnobController = nullptr;

	float m_Value = 0;
	std::function<void(float)> m_OnSliderChange;
	float m_MinValue = 0;
	float m_MaxValue = 0;

};