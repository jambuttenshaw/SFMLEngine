#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

class ScrollToControlLight : public ScriptableEntity
{
public:
	void Start() override
	{
		m_Light = &GetComponent<PointLight>();
	}

	void Update(Timestep ts) override
	{
		m_Light->SetRange(m_Light->GetRange() - (m_ScrollSpeed * Input::GetMouseWheelDelta() * ts));
	}

private:
	PointLight* m_Light = nullptr;

	float m_ScrollSpeed = 750.0f;
};
