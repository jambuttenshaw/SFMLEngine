#pragma once

#include <SFMLEngine.h>
#include "Fader.h"

using namespace SFMLEngine;



class FadeInController : public ScriptableEntity
{
public:
	void Start() override
	{
		// create a new fader entity
		m_FaderEntity = CreateEntity();
		m_Fader = &AddNativeScript<Fader>(m_FaderEntity);
		// set it to fade out (thus the scene fades in)
		m_Fader->SetFadeState(Fader::State::FadeOut);
	}

	void Update(float ts) override
	{
		// once the fade has completed
		if (m_Fader->FadeComplete())
		{
			// destroy both the fader entity and this entity
			Destroy(m_FaderEntity);
			Destroy();
		}
	}


private:
	Entity m_FaderEntity = INVALID_ENTITY_ID;
	Fader* m_Fader = nullptr;
};
