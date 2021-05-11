#pragma once

#include <SFMLEngine.h>
#include "menu/FadeInController.h"
#include "menu/GoToMouse.h"
#include "deathScreen/DeathScreenButtons.h"

#include "Constants.h"
#include "PlayerData.h"


using namespace SFMLEngine;


/*
The win screen displays when the player returns to the surface and is presented with their score
*/

class WinScreen : public Scene
{
public:

	void Create() override
	{
		/*
		Buttons need to be placed onto the win screen:
		- A replay button
		- A exit to menu button

		Buttons are implemented as in-game sprites rather than on the GUI.
		They therefore requrie:
		- A transform
		- A SpriteRenderer
		- A collider
		- A button script
		*/

		{
			Entity replayButton = CreateEntity();

			AddComponent(replayButton, Transform{ { -180, 113 } });
			AddComponent(replayButton, SpriteRenderer{
					Texture::Create("assets/textures/buttons/replayButton.png"),
					Material::Create("Lit"),
					0,
					Texture::Create("assets/textures/buttons/replayButton_n.png"),
				});

			AddComponent(replayButton, BoxCollider{ {160, 64}, {0, 0} });
			AddComponent(replayButton, ColliderInfo{ ColliderType::Box });

			AddNativeScript<ReplayButton>(replayButton);
		}
		{
			Entity menuButton = CreateEntity();

			AddComponent(menuButton, Transform{ { 20, 113 } });
			AddComponent(menuButton, SpriteRenderer{
					Texture::Create("assets/textures/buttons/menuButton.png"),
					Material::Create("Lit"),
					0,
					Texture::Create("assets/textures/buttons/menuButton_n.png"),
				});

			AddComponent(menuButton, BoxCollider{ {128, 64}, {0, 0} });
			AddComponent(menuButton, ColliderInfo{ ColliderType::Box });

			AddNativeScript<ExitToMenuButton>(menuButton);
		}

		// create big text telling the player they made it out of the cave
		Entity mainText = CreateEntity();
		AddComponent(mainText, Transform{ { -189, -200 } });
		AddComponent(mainText, SpriteRenderer{
			Texture::Create("assets/textures/winText.png"),
			Material::Create("Lit"),
			0,
			Texture::Create("assets/textures/winText_n.png"),
			});


		// text to tell them what their scores are
		Entity scoreText = CreateEntity();
		AddComponent(scoreText, Transform{ { -189.0f, -36.5f } });
		AddComponent(scoreText, SpriteRenderer{
			Texture::Create("assets/textures/scoreText.png"), Material::Create("Basic"), 0 });


		// every scene needs a camera
		// in this scene the player does not manipulate the camera
		Entity camera = CreateEntity();
		SetEntityTag(camera, "Camera");
		AddComponent(camera, Transform{ });
		AddComponent(camera, Camera{ });


		// this scene should fade in from the main game scene
		// to make the transition look smooth
		Entity fadeInController = CreateEntity();
		AddNativeScript<FadeInController>(fadeInController);


		// create some neat lighting to make the scene look cool
		// add a point light that follows the mouse (via a script)
		// and a directional lighting to make it not completely black when the user hovers their mouse elsewhere
		Entity light = CreateEntity();
		SetEntityTag(light, "SceneLight");
		AddComponent(light, Transform{ });
		AddComponent(light, PointLight{ 3.0f, 400.0f, sf::Color(219, 113, 114, 255) });
		AddNativeScript<GoToMouse>(light);
		// place both lights onto the same entity
		AddComponent(light, DirectionalLight{ 0.0f, -1.7f, 0.3f, sf::Color(200, 61, 19, 255) });



		// load the player data
		// the data store keeps data stored between scenes
		// as we cannot place data onto an entity since the entity would be destroyed between scene unloading and reloading
		PlayerData* data = DataStore::RetrieveData<PlayerData>("playerData");
		// they get a score bonus for every half-heart they retained
		int healthBonus = HEART_BONUS_POINTS * data->Health;
		// the depth they travelled into the cave becomes a score mulitplier to their crystal score
		int depthBonus = static_cast<int>(data->Score * (1 + data->DepthPercent));
		int totalScore = data->Score + healthBonus + depthBonus;
		
		// create text that displays all of this info on the display
		CreateScoreText(data->Score, { 90.0f, -30.5f });
		CreateScoreText(depthBonus, { 90.0f, 1.5f });
		CreateScoreText(healthBonus, { 90.0f, 33.5f });
		CreateScoreText(totalScore, { 90.0f, 65.5f });
		
	}

	Entity CreateScoreText(int score, const sf::Vector2f& pos)
	{
		Entity newScoreText = CreateEntity();

		// renders text onto a texture that the entity then displays using the sprite renderer component
		AddComponent(newScoreText, Transform{ pos });
		AddComponent(newScoreText, SpriteRenderer{
			Texture::Create(FontLibrary::GetFont("arcade"), std::to_string(score), 24, sf::Color::White), Material::Create("Basic"), 0 });

		return newScoreText;
	}
};
