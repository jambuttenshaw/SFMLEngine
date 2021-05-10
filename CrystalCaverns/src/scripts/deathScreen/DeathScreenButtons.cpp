#include "DeathScreenButtons.h"

#include "../../src/MainMenu.h"
#include "../../src/MainScene.h"
#include "../../src/levels/Level1.h"


void ReplayButton::OnClick()
{
	// when the replay button is clicked, the application should load the level again
	
	// due to the way scene loading works, Level1 needs to be queued for loading before the MainScene
	// this is just a slightly annoying consequence of the implementation of additive scene loading
	Application::GetApplicationHandle()->LoadScene<Level1>(LoadSceneMode::Additive);
	Application::GetApplicationHandle()->LoadScene<MainScene>(LoadSceneMode::Single);
}

void ExitToMenuButton::OnClick()
{
	// returning to the main menu is simple enough: just load the menu scene
	Application::GetApplicationHandle()->LoadScene<MainMenu>(LoadSceneMode::Single);
}
