#pragma once

#include <SFMLEngine.h>
#include "Button.h"

using namespace SFMLEngine;


class ReplayButton : public Button
{
public:

	virtual void OnClick() override;
};


class ExitToMenuButton : public Button
{
public:

	virtual void OnClick() override;
	
};
