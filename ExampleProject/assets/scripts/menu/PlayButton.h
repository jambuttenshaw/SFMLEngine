#pragma once

#include <SFMLEngine.h>
#include "Button.h"

using namespace SFMLEngine;


class MainScene;
class Tutorial;
class Level1;


class PlayButton : public Button
{
public:
	void Start() override;
	void Update(float ts) override;

	void OnClick() override;

private:
	PointLight* m_SceneLight;
	DirectionalLight* m_DirLight;

	float m_InitialRange = 0.0f;
	float m_InitialIntensity = 0.0f;

	bool m_Fading = false;
	float m_FadeTime = 0.5f;
	float m_FadeTimer = 0.0f;

};
