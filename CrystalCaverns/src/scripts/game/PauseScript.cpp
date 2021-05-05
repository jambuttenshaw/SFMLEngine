#include "PauseScript.h"

#include "PauseMenu.h"
#include "pauseMenu/ClosePauseMenu.h"

bool PauseScript::s_IsPaused = false;


void PauseScript::Pause()
{
	s_IsPaused = true;
	Application::GetApplicationHandle()->SetTimeScale(0);

	// pause all currently playing sounds
	AudioSystem::PauseAllSounds();
	// because pausing all the sounds will stop the click being heard,
	// we make it click again
	AudioSystem::PlaySound("buttonClick");

	// open the pause menu
	Application::GetApplicationHandle()->LoadScene<PauseMenu>(LoadSceneMode::Additive);
}

void PauseScript::Resume()
{
	s_IsPaused = false;
	Application::GetApplicationHandle()->SetTimeScale(1);

	// resume all sounds that were paused
	AudioSystem::ResumeAllSounds();

	// close the pause menu
	GetNativeScript<ClosePauseMenu>(GetEntitiesWithTag("ClosePauseMenu")[0]).Close();
}