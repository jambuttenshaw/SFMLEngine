#pragma once

#include <SFMLEngine.h>
#include "Button.h"

using namespace SFMLEngine;

#include "MenuCameraController.h"


class OptionsButton : public Button
{
public:

	void Start() override
	{
		m_CameraController = &GetNativeScript<MenuCameraController>(GetEntitiesWithTag("Camera")[0]);
	}

	virtual void OnClick() override
	{
		m_CameraController->SetTarget({ 0, 896 });
	}

private:
	MenuCameraController* m_CameraController = nullptr;

};
