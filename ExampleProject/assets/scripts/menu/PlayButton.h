#pragma once

#include <SFMLEngine.h>
#include "Button.h"

using namespace SFMLEngine;


class MainScene;


class PlayButton : public Button
{
public:

	virtual void OnClick() override
	{
		Application::GetApplicationHandle()->LoadScene<MainScene>();
	}

};
