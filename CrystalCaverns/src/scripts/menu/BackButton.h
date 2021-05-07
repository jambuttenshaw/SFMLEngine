#pragma once

#include <SFMLEngine.h>
#include "Button.h"

using namespace SFMLEngine;

#include "MenuCameraController.h"


class BackButton : public Button
{
public:

	void Start() override
	{
		m_CameraController = &GetNativeScript<MenuCameraController>(GetEntitiesWithTag("Camera")[0]);
	}

	virtual void OnClick() override
	{
		m_CameraController->SetTarget({ 0, 0 });
	}



private:
	MenuCameraController* m_CameraController;

};