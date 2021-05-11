#pragma once

#include <SFMLEngine.h>
#include "Button.h"

using namespace SFMLEngine;


class QuitButton : public Button
{
public:

	virtual void OnClick() override
	{
		// close the application when the quit button is pressed
		Application::GetApplicationHandle()->Close();
	}

};
