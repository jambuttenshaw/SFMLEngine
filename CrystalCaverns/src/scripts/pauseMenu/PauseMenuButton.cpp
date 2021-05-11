#include "PauseMenuButton.h"

#include "game/PauseScript.h"
#include "MainMenu.h"



void PResumeButton::OnClick()
{
	// when the resume button is clicked,
	// we call the resume function in the pause script
	// as this button is about to be deleted, it shouldnt be in charge of resuming the game
	GetNativeScript<PauseScript>(GetEntitiesWithTag("Player")[0]).Resume();
}

void PMenuButton::OnClick()
{
	// returns to menu
	// first of all resumes the game, to fix stuff like the sounds and the time scale
	GetNativeScript<PauseScript>(GetEntitiesWithTag("Player")[0]).Resume();
	// and then loads the menu scene
	Application::GetApplicationHandle()->LoadScene<MainMenu>(LoadSceneMode::Single);
}

void PQuitButton::OnClick()
{
	// close the application
	Application::GetApplicationHandle()->Close();
}
