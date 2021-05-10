#pragma once

#include <SFMLEngine.h>
#include "scripts/game/WolfController.h"

using namespace SFMLEngine;


/*
The wolf is the main enemy in Crystal Caverns
This factory function sets up a wolf entity with the required components and data
*/
class Wolf
{
public:
	static Entity Create(Scene* scene, const sf::Vector2f& position)
	{
		// create a new entity
		Entity newWolf = scene->CreateEntity();
		// assign its physics layer
		// we dont want the player to collide with wolves, or wolves to collide with other wolves
		// this is used by filtering collisions by physics layer
		scene->SetEntityLayer(newWolf, "Enemies");
		// set the entities tag so we can identify it as a wolf
		scene->SetEntityTag(newWolf, "Wolf");

		// place the wolf at the specified position
		scene->AddComponent(newWolf, Transform{ position });

		// add the sprite renderer component
		scene->AddComponent(newWolf, SpriteRenderer{
			Texture::Create("assets/textures/wolfSheet.png"),
			Material::Create("Lit"),
			2,
			Texture::Create("assets/textures/wolfSheet_n.png") });

		// wolves are affected by physics
		// add a rigidbody component
		scene->AddComponent(newWolf, Rigidbody{ });
		// add a collider to the wolf
		scene->AddComponent(newWolf, BoxCollider{ sf::Vector2f(64, 29), sf::Vector2f(0, 3) });
		scene->AddComponent(newWolf, ColliderInfo{ ColliderType::Box });

		// the wolf controller is in charge of all of the wolf's behaviour
		scene->AddNativeScript<WolfController>(newWolf);

		// define all of the animations the wolf uses
		// there are a lot of these, one for each state of the wolf
		Animation idle{ "idle", {
			{ 0, 0, 64, 32 } },
			0.1f
		}; idle.SetLooping(false);

		Animation sleep{ "sleep", {
			{ 0,   0, 64, 32 },
			{ 64,  0, 64, 32 },
			{ 128, 0, 64, 32 },
			{ 192, 0, 64, 32 } },
			0.1f
		}; sleep.SetLooping(false);

		Animation wake{ "wake", {
			{ 192, 0, 64, 32 },
			{ 128, 0, 64, 32 },
			{ 64,  0, 64, 32 },
			{ 0,   0, 64, 32 } },
			0.05f
		}; wake.SetLooping(false);

		Animation alert{ "alert", {
			{ 0,   64, 64, 32,  0,  0 },
			{ 64,  59, 64, 37,  0, -5 },
			{ 128, 55, 64, 41,  0, -9 },
			{ 192, 55, 64, 41,  0, -9 } },
			0.1f
		}; alert.SetLooping(false);

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
		}; bite.SetLooping(false);

		Animation climb{ "climb", {
			{ 320, 256, 32, 64 },
			{ 320, 192, 32, 64 },
			{ 320, 128, 32, 64 },
			{ 320, 64,  32, 64 },
			{ 320, 0,   32, 64 } },
			0.1f
		};

		// create the animator component and add all of the animations
		Animator animator{ AnimableType::Sprite };
		animator.AddAnimation(idle);
		animator.AddAnimation(sleep);
		animator.AddAnimation(wake);
		animator.AddAnimation(alert);
		animator.AddAnimation(walk);
		animator.AddAnimation(run);
		animator.AddAnimation(bite);
		animator.AddAnimation(climb);

		// just set the wolf to idle by default
		animator.SetCurrentAnimation("idle");
		scene->AddComponent(newWolf, animator);

		// return the wolf entity handle 
		return newWolf;
	}
};
