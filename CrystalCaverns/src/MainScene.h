#pragma once

#include <SFMLEngine.h>

// scripts
#include "game/PlayerController.h"
#include "game/PlayerStatsController.h"
#include "game/CrystalCollector.h"
#include "game/PlayerLightAnimator.h"

#include "game/DepthFinder.h"

#include "game/CameraController.h"
#include "game/BackgroundScript.h"

#include "game/WolfManager.h"

#include "game/LevelManager.h"
#include "game/PauseScript.h"

#include "entities/PlayerHeart.h"


using namespace SFMLEngine;


/*
The Main Scene contains all of the stuff that should exist in every level
such as the player, the camera, ambient lighting, etc...
*/


class MainScene : public Scene
{
public:
	void Create() override
	{

		{
			// create the player
			// its name, tag and layer should all be Player
			m_Player = CreateEntity("Player", "Player", "Player");

			// give the entity a transform
			AddComponent(m_Player, Transform{ sf::Vector2f(0, -200) });

			// add a rigidbody so this entity is affected by physics
			AddComponent(m_Player, Rigidbody{ });
			// the player also has a collider so it collides with other objects
			AddComponent(m_Player, BoxCollider{ sf::Vector2f(17, 48), sf::Vector2f(8, 16) });
			AddComponent(m_Player, ColliderInfo{ ColliderType::Box });

			// add the sprite renderer component
			// the player is rendered with a lit material
			AddComponent(m_Player, SpriteRenderer{
				Texture::Create("assets/textures/characterSheet.png"),
				Material::Create("Lit"),
				1,
				Texture::Create("assets/textures/characterSheetNormals.png") });


			// create the animations and animator component
			// an animation object contains a string to identify the animation,
			// and a list of rectangles that describe the position of the frame on the spritesheet
			Animation idle{ "idle", {
				{0,   0, 32, 64},
				{34,  0, 32, 64},
				{66,  0, 32, 64},
				{96,  0, 32, 64},
				{129, 0, 32, 64},
				{161, 0, 32, 64} },
				0.1f
			};
			Animation run{ "run", {
				{1,   128, 32, 64},
				{33,  128, 32, 64},
				{64,  128, 32, 64},
				{96,  128, 32, 64},
				{128, 128, 32, 64},
				{160, 128, 32, 64} },
				0.1f
			};
			Animation jump{ "jump", {
				{0,  320, 32, 64},
				{32, 320, 32, 64},
				{64, 320, 32, 64} },
				0.1f
			}; jump.SetLooping(false);
			Animation climb{ "climb", {
				{0, 510, 32, 64},
				{29, 510, 32, 64},
				{63, 510, 32, 64},
				{93, 510, 32, 64},
				{126, 510, 32, 64} },
				0.15f
			};
			Animation idleCrawl{ "idleCrawl", {
				{0, 288, 64, 32} },
				0.1f
			}; idleCrawl.SetLooping(false);
			Animation crawl{ "crawl", {
				{0, 288, 64, 32},
				{61, 288, 64, 32},
				{126, 288, 64, 32},
				{192, 288, 64, 32},
				{253, 288, 64, 32},
				{317, 288, 64, 32},
				{384, 288, 64, 32} },
				0.1f
			};
			Animation hurt{ "hurt", {
				{0, 1024, 32, 64},
				{30, 1024, 32, 64},
				{63, 1024, 32, 64},
				{97, 1024, 32, 64},
				{128, 1024, 32, 64} },
				0.09f
			}; hurt.SetLooping(false);
			Animation die{ "die", {
				{0, 1088, 32, 64},
				{32, 1088, 32, 64},
				{64, 1088, 42, 64},
				{103, 1088, 42, 64},
				{146, 1088, 42, 64},
				{196, 1088, 48, 64} },
				0.1f
			}; die.SetLooping(false);

			// create an animator and add all of the animations to it
			Animator animator{ AnimableType::Sprite };
			animator.AddAnimation(idle);
			animator.AddAnimation(run);
			animator.AddAnimation(jump);
			animator.AddAnimation(climb);
			animator.AddAnimation(idleCrawl);
			animator.AddAnimation(crawl);
			animator.AddAnimation(hurt);
			animator.AddAnimation(die);

			// set an initial animation for the player
			animator.SetCurrentAnimation("idle");
			// and add the animator to the player
			AddComponent(m_Player, animator);

			// then add the players scripts

			// add the player controller
			// the player controller controls the players movement, mechanics and physical interaction with other objects
			AddNativeScript<PlayerController>(m_Player);


			// quickly create the score GUI
			// just a small text label that contains the score
			{
				m_CrystalScoreText = CreateEntity();
				AddComponent(m_CrystalScoreText, GUITransform{ { 0.01f, 0.1f }, GUIElementType::Text });
				AddComponent(m_CrystalScoreText, GUIText{ FontLibrary::GetFont("arcade"), "", 32, sf::Color::White });
			}
			// add a crystal collector script to the player
			// this deals with interacting with the crystals and handling the players score
			// it also updates the gui text
			auto& crystalScript = AddNativeScript<CrystalCollector>(m_Player);
			crystalScript.SetScoreText(m_CrystalScoreText);
			crystalScript.SetScenePtr(this);
			

			// create 3 hearts that display the players health
			// the hearts are created by another script
			for (int i = 0; i < 3; i++)
			{
				m_PlayerHearts.push_back(PlayerHeart::Create(this, { 0.05f * i, 0 }));
			}
			// add a player stats controller script
			// this controls the players health and updates the hearts gui images
			auto& statScript = AddNativeScript<PlayerStatsController>(m_Player);
			statScript.SetupHearts(&m_PlayerHearts);


			// the player is the host for the level manager script
			// due to the fact that it is the player that triggers when the game will change level
			auto& levelManager = AddNativeScript<LevelManager>(m_Player);
			levelManager.SetMainScene(this);


			// the player also contains the pause script
			AddNativeScript<PauseScript>(m_Player);
			// add a script that animates the players light when it interacts with objects to make it cool
			AddNativeScript<PlayerLightAnimator>(m_Player);
		}

		{
			// create the scene light
			m_Light = CreateEntity();
			SetEntityTag(m_Light, "PlayerLight");

			// light is a child transform of the player
			// this lets the light move with the player without having to add a script
			AddComponent(m_Light, Transform{ sf::Vector2f(16, 32), &GetComponent<Transform>(m_Player) });
			AddComponent(m_Light, PointLight{ 3.2f, 225.0f, sf::Color(219, 113, 114, 255) });
		}

		{
			// add the camera to the scene
			m_Camera = CreateEntity();
			SetEntityTag(m_Camera, "MainCamera");

			AddComponent(m_Camera, Transform{ });

			// zoom it in a bit for effect
			Camera cam{ };
			cam.SetZoom(0.6f);
			AddComponent(m_Camera, cam);

			// add a controller script that makes the camera smoothly follow the player
			// and also does the camera shake effects
			AddNativeScript<CameraController>(m_Camera);
		}

		{
			// create an entity to generate and control the parallax background
			m_Background = CreateEntity();
			AddNativeScript<BackgroundScript>(m_Background);
			
		}

		{
			// create the wolf manager
			// even though no wolves are contained in this scene,
			// when a new scene loads the wolf manager finds all the new wolves to keep track of
			m_WolfManager = CreateEntity();
			SetEntityTag(m_WolfManager, "WolfManager");
			AddNativeScript<WolfManager>(m_WolfManager);
		}


		{
			// create the gui bar at the side that keeps track of how deep the player has explored into the cave
			m_DepthBar = CreateEntity();
			SetEntityTag(m_DepthBar, "DepthBar");

			// give it an image and position it
			// the anchor is adjusted from the topleft to the centre
			GUITransform t{ {0.97f, 0.5f}, GUIElementType::Image };
			t.SetHorizontalAnchor(GUITransform::Anchor::Middle);
			t.SetVerticalAnchor(GUITransform::Anchor::Middle);
			AddComponent(m_DepthBar, t);


			// add the depth bar texture
			AddComponent(m_DepthBar, GUIImage{ Texture::Create("assets/textures/depthBar.png"), 0 });

			// add the script that controls the markers on the bar
			AddNativeScript<DepthFinder>(m_DepthBar);
		}



		// players dont collide with enemies in terms of physics
		// and enemies shouldn't collide with other enemies
		Physics::IgnoreCollisions("Player", "Enemies");
		Physics::IgnoreCollisions("Enemies", "Enemies");
	}

private:
	Entity m_Camera = INVALID_ENTITY_ID;
	Entity m_Background = INVALID_ENTITY_ID;

	Entity m_Player = INVALID_ENTITY_ID;
	 

	Entity m_Light = INVALID_ENTITY_ID;

	std::vector<Entity> m_PlayerHearts;
	Entity m_CrystalScoreText = INVALID_ENTITY_ID;
	Entity m_DepthBar = INVALID_ENTITY_ID;


	Entity m_WolfManager = INVALID_ENTITY_ID;
};

