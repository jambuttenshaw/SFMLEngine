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
	PointLight* m_SceneLight = nullptr;
	PointLight* m_TitleLight = nullptr;
	DirectionalLight* m_DirLight = nullptr;

	// used for interpolating the properties of the scene lights when the play button is clicked
	float m_InitialRange = 0.0f;
	float m_InitialIntensity1 = 0.0f;
	float m_InitialIntensity2 = 0.0f;

	bool m_Fading = false;
	// how long will the menu take to fade out
	float m_FadeTime = 0.5f;
	float m_FadeTimer = 0.0f;

};
