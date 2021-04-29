#include "DeathScreenButtons.h"

#include "../../src/MainMenu.h"
#include "../../src/MainScene.h"
#include "../../src/levels/Level1.h"


void ReplayButton::OnClick()
{
	Application::GetApplicationHandle()->LoadScene<Level1>(LoadSceneMode::Additive);
	Application::GetApplicationHandle()->LoadScene<MainScene>(LoadSceneMode::Single);
}

void ExitToMenuButton::OnClick()
{
	Application::GetApplicationHandle()->LoadScene<MainMenu>(LoadSceneMode::Single);
}
