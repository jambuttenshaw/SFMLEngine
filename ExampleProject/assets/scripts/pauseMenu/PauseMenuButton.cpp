#include "PauseMenuButton.h"

#include "game/PauseScript.h"
#include "../../ExampleProject/src/MainMenu.h"



void PResumeButton::OnClick()
{
	GetNativeScript<PauseScript>(GetEntitiesWithTag("Player")[0]).Resume();
}

void PMenuButton::OnClick()
{
	GetNativeScript<PauseScript>(GetEntitiesWithTag("Player")[0]).Resume();
	Application::GetApplicationHandle()->LoadScene<MainMenu>(LoadSceneMode::Single);
}

void PQuitButton::OnClick()
{
	Application::GetApplicationHandle()->Close();
}
