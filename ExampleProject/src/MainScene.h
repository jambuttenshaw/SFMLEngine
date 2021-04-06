#pragma once

#include <SFMLEngine.h>

// scripts
#include "game/PlayerController.h"
#include "game/PlayerStatsController.h"
#include "game/CrystalCollector.h"

#include "game/SmoothFollowPlayer.h"

#include "entities/PlayerHeart.h"


#include "levels/Level1.h"


using namespace SFMLEngine;


class MainScene : public Scene
{
public:
	void Create() override
	{
		ResourceID fontID = FontLibrary::LoadNewFont("arcade", "assets/fonts/ARCADECLASSIC.TTF");

		{
			// create the player
			// its name, tag and layer should all be Player
			m_Player = CreateEntity("Player", "Player", "Player");

			// give the entity a transform
			AddComponent(m_Player, Transform{ sf::Vector2f(0, -200) });

			// add a rigidbody so this entity is affected by physics
			AddComponent(m_Player, Rigidbody{ });
			AddComponent(m_Player, BoxCollider{ sf::Vector2f(17, 48), sf::Vector2f(8, 16) });
			AddComponent(m_Player, ColliderInfo{ ColliderType::Box });

			// add the sprite renderer component
			AddComponent(m_Player, SpriteRenderer{
				Texture::Create("assets/textures/characterSheet.png"),
				Material::Create("Lit"),
				2,
				Texture::Create("assets/textures/characterSheetNormals.png") });


			// create the animations and animator component
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
			}; jump.Looping = false;
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
			}; idleCrawl.Looping = false;
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
			}; hurt.Looping = false;

			Animator animator{ AnimableType::Sprite };
			animator.AddAnimation(idle);
			animator.AddAnimation(run);
			animator.AddAnimation(jump);
			animator.AddAnimation(climb);
			animator.AddAnimation(idleCrawl);
			animator.AddAnimation(crawl);
			animator.AddAnimation(hurt);
			animator.SetCurrentAnimation("idle");

			AddComponent(m_Player, animator);


			AddNativeScript<PlayerController>(m_Player);


			{
				m_CrystalScoreText = CreateEntity();
				AddComponent(m_CrystalScoreText, GUITransform{ { 0.01f, 0.1f }, GUIElementType::Text });
				AddComponent(m_CrystalScoreText, GUIText{ fontID, "", 32, sf::Color::White });
			}
			auto& crystalScript = AddNativeScript<CrystalCollector>(m_Player);
			crystalScript.SetScoreText(m_CrystalScoreText);
			

			for (int i = 0; i < 3; i++)
			{
				m_PlayerHearts.push_back(PlayerHeart::Create(this, { 0.05f * i, 0 }));
			}
			auto& statScript = AddNativeScript<PlayerStatsController>(m_Player);
			statScript.SetupHearts(&m_PlayerHearts);
		}

		{
			m_Light = CreateEntity();

			// light is a child transform of physics entity
			AddComponent(m_Light, Transform{ sf::Vector2f(16, 32), &GetComponent<Transform>(m_Player) });
			AddComponent(m_Light, PointLight{ 3.0f, 350.0f, sf::Color(219, 113, 114, 255) });
		}

		{
			m_Camera = CreateEntity();

			AddComponent(m_Camera, Transform{ });

			Camera cam{ };
			cam.Zoom = 0.75f;
			AddComponent(m_Camera, cam);

			AddNativeScript<SmoothFollowPlayer>(m_Camera);
		}

		Physics::IgnoreCollisions("Player", "Enemies");
	}

private:
	Entity m_Camera = INVALID_ENTITY_ID;

	Entity m_Player = INVALID_ENTITY_ID;
	 

	Entity m_Light = INVALID_ENTITY_ID;

	std::vector<Entity> m_PlayerHearts;
	Entity m_CrystalScoreText = INVALID_ENTITY_ID;

};

