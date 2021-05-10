#pragma once

#include <SFMLEngine.h>
#include "scripts/Button.h"

using namespace SFMLEngine;

/*
The buttons on the death screen are just implementations of the Button base class

They just need their own derived class for implementing the logic that happens when they are clicked
*/
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
