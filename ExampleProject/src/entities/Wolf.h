#pragma once

#include <SFMLEngine.h>
#include "game/WolfController.h"

using namespace SFMLEngine;


class Wolf
{
public:
	static Entity Create(Scene* scene, const sf::Vector2f& position)
	{
		Entity newWolf = scene->CreateEntity();
		scene->SetEntityLayer(newWolf, "Enemies");
		scene->SetEntityTag(newWolf, "Wolf");

		scene->AddComponent(newWolf, Transform{ position });

		// add the sprite renderer component
		scene->AddComponent(newWolf, SpriteRenderer{
			Texture::Create("assets/textures/wolfSheet.png"),
			Material::Create("Lit"),
			2,
			Texture::Create("assets/textures/wolfSheet_n.png") });

		scene->AddComponent(newWolf, Rigidbody{ });
		scene->AddComponent(newWolf, BoxCollider{ sf::Vector2f(64, 29), sf::Vector2f(0, 3) });
		scene->AddComponent(newWolf, ColliderInfo{ ColliderType::Box });

		scene->AddNativeScript<WolfController>(newWolf);

		Animation idle{ "idle", {
			{ 0, 0, 64, 32 } },
			0.1f
		}; idle.Looping = false;

		Animation sleep{ "sleep", {
			{ 0,   0, 64, 32 },
			{ 64,  0, 64, 32 },
			{ 128, 0, 64, 32 },
			{ 192, 0, 64, 32 } },
			0.1f
		}; sleep.Looping = false;

		Animation wake{ "wake", {
			{ 192, 0, 64, 32 },
			{ 128, 0, 64, 32 },
			{ 64,  0, 64, 32 },
			{ 0,   0, 64, 32 } },
			0.05f
		}; wake.Looping = false;

		Animation alert{ "alert", {
			{ 0,   64, 64, 32,  0,  0 },
			{ 64,  59, 64, 37,  0, -5 },
			{ 128, 55, 64, 41,  0, -9 },
			{ 192, 55, 64, 41,  0, -9 } },
			0.1f
		}; alert.Looping = false;

		Animation walk{ "walk", {
			{ 0,   96, 64, 32 },
			{ 64,  96, 64, 32 },
			{ 128, 96, 64, 32 },
			{ 192, 96, 64, 32 },
			{ 256, 96, 64, 32 } },
			0.1f
		};

		Animation run{ "run", {
			{ 0,   128, 64, 32 },
			{ 64,  128, 64, 32 },
			{ 128, 128, 64, 32 },
			{ 192, 128, 64, 32 },
			{ 256, 128, 64, 32 } },
			0.1f
		};

		Animation bite{ "bite", {
			{ 0,   160, 64, 32 },
			{ 64,  160, 64, 32 },
			{ 128, 160, 64, 32 },
			{ 192, 160, 64, 32 },
			{ 256, 160, 64, 32 } },
			0.1f
		}; bite.Looping = false;

		Animation climb{ "climb", {
			{ 320, 256, 32, 64 },
			{ 320, 192, 32, 64 },
			{ 320, 128, 32, 64 },
			{ 320, 64,  32, 64 },
			{ 320, 0,   32, 64 } },
			0.1f
		};

		Animator animator{ AnimableType::Sprite };
		animator.AddAnimation(idle);
		animator.AddAnimation(sleep);
		animator.AddAnimation(wake);
		animator.AddAnimation(alert);
		animator.AddAnimation(walk);
		animator.AddAnimation(run);
		animator.AddAnimation(bite);
		animator.AddAnimation(climb);

		animator.SetCurrentAnimation("idle");
		scene->AddComponent(newWolf, animator);

		return newWolf;
	}
};
