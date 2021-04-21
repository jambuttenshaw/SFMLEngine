#pragma once

#include <SFMLEngine.h>
#include "Fader.h"

using namespace SFMLEngine;



class FadeInController : public ScriptableEntity
{
public:
	void Start() override
	{
		m_FaderEntity = CreateEntity();
		m_Fader = &AddNativeScript<Fader>(m_FaderEntity);
		m_Fader->SetFadeState(Fader::State::FadeOut);
	}

	void Update(float ts) override
	{
		if (m_Fader->FadeComplete())
		{
			Destroy(m_FaderEntity);
			Destroy();
		}
	}


private:
	Entity m_FaderEntity = INVALID_ENTITY_ID;
	Fader* m_Fader = nullptr;
};
