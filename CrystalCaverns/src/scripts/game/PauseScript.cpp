#include "PauseScript.h"

#include "PauseMenu.h"
#include "pauseMenu/ClosePauseMenu.h"

bool PauseScript::s_IsPaused = false;


void PauseScript::Pause()
{
	// pause the  game

	// mark the game as paused
	s_IsPaused = true;
	// stop time so no physics or animations progress
	Application::GetApplicationHandle()->SetTimeScale(0);

	// pause all currently playing sounds
	AudioSystem::PauseAllSounds();

	// open the pause menu
	Application::GetApplicationHandle()->LoadScene<PauseMenu>(LoadSceneMode::Additive);
}

void PauseScript::Resume()
{
	// start time again
	Application::GetApplicationHandle()->SetTimeScale(1);

	// resume all sounds that were paused
	AudioSystem::ResumeAllSounds();

	// close the pause menu
	GetNativeScript<ClosePauseMenu>(GetEntitiesWithTag("ClosePauseMenu")[0]).Close();

	// mark as no longer paused
	s_IsPaused = false;
}