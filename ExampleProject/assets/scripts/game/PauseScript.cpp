#include "PauseScript.h"

#include "../../src/PauseMenu.h"
#include "pauseMenu/ClosePauseMenu.h"

bool PauseScript::s_IsPaused = false;


void PauseScript::Pause()
{
	s_IsPaused = true;
	Application::GetApplicationHandle()->SetTimeScale(0);

	// open the pause menu
	Application::GetApplicationHandle()->LoadScene<PauseMenu>(LoadSceneMode::Additive);
}

void PauseScript::Resume()
{
	s_IsPaused = false;
	Application::GetApplicationHandle()->SetTimeScale(1);

	// close the pause menu
	GetNativeScript<ClosePauseMenu>(GetEntitiesWithTag("ClosePauseMenu")[0]).Close();
}