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
			m_ReplayButton = CreateEntity();

			AddComponent(m_ReplayButton, Transform{ { -180, 113 } });
			AddComponent(m_ReplayButton, SpriteRenderer{
					Texture::Create("assets/textures/buttons/replayButton.png"),
					Material::Create("Lit"),
					0,
					Texture::Create("assets/textures/buttons/replayButton_n.png"),
				});

			AddComponent(m_ReplayButton, BoxCollider{ {160, 64}, {0, 0} });
			AddComponent(m_ReplayButton, ColliderInfo{ ColliderType::Box });

			AddNativeScript<ReplayButton>(m_ReplayButton);
		}
		{
			m_MenuButton = CreateEntity();

			AddComponent(m_MenuButton, Transform{ { 20, 113 } });
			AddComponent(m_MenuButton, SpriteRenderer{
					Texture::Create("assets/textures/buttons/menuButton.png"),
					Material::Create("Lit"),
					0,
					Texture::Create("assets/textures/buttons/menuButton_n.png"),
				});

			AddComponent(m_MenuButton, BoxCollider{ {128, 64}, {0, 0} });
			AddComponent(m_MenuButton, ColliderInfo{ ColliderType::Box });

			AddNativeScript<ExitToMenuButton>(m_MenuButton);
		}

		// create big text telling the player they made it out of the cave
		m_MainText = CreateEntity();
		AddComponent(m_MainText, Transform{ { -189, -200 } });
		AddComponent(m_MainText, SpriteRenderer{
			Texture::Create("assets/textures/winText.png"),
			Material::Create("Lit"),
			0,
			Texture::Create("assets/textures/winText_n.png"),
			});


		// text to tell them what their scores are
		m_ScoreText = CreateEntity();
		AddComponent(m_ScoreText, Transform{ { -189.0f, -36.5f } });
		AddComponent(m_ScoreText, SpriteRenderer{
			Texture::Create("assets/textures/scoreText.png"), Material::Create("Basic"), 0 });


		// every scene needs a camera
		// in this scene the player does not manipulate the camera
		m_Camera = CreateEntity();
		SetEntityTag(m_Camera, "Camera");
		AddComponent(m_Camera, Transform{ });
		AddComponent(m_Camera, Camera{ });


		// this scene should fade in from the main game scene
		// to make the transition look smooth
		m_FadeInController = CreateEntity();
		AddNativeScript<FadeInController>(m_FadeInController);


		// create some neat lighting to make the scene look cool
		// add a point light that follows the mouse (via a script)
		// and a directional lighting to make it not completely black when the user hovers their mouse elsewhere
		m_Light = CreateEntity();
		SetEntityTag(m_Light, "SceneLight");
		AddComponent(m_Light, Transform{ });
		AddComponent(m_Light, PointLight{ 3.0f, 400.0f, sf::Color(219, 113, 114, 255) });
		AddNativeScript<GoToMouse>(m_Light);
		// place both lights onto the same entity
		AddComponent(m_Light, DirectionalLight{ 0.0f, -1.7f, 0.3f, sf::Color(200, 61, 19, 255) });



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

private:
	Entity m_MainText = INVALID_ENTITY_ID;
	Entity m_ScoreText = INVALID_ENTITY_ID;
	Entity m_Camera = INVALID_ENTITY_ID;
	Entity m_FadeInController = INVALID_ENTITY_ID;

	Entity m_ReplayButton = INVALID_ENTITY_ID;
	Entity m_MenuButton = INVALID_ENTITY_ID;

	Entity m_Light = INVALID_ENTITY_ID;
};
