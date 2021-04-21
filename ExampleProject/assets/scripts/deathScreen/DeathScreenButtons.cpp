#include "DeathScreenButtons.h"

#include "../../ExampleProject/src/MainMenu.h"
#include "../../ExampleProject/src/MainScene.h"
#include "../../ExampleProject/src/levels/Level1.h"


void ReplayButton::OnClick()
{
	Application::GetApplicationHandle()->LoadScene<Level1>(LoadSceneMode::Additive);
	Application::GetApplicationHandle()->LoadScene<MainScene>(LoadSceneMode::Single);
}

void ExitToMenuButton::OnClick()
{
	Application::GetApplicationHandle()->LoadScene<MainMenu>(LoadSceneMode::Single);
}
