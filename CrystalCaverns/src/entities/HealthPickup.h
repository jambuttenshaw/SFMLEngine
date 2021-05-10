#pragma once

#include <SFMLEngine.h>
#include "scripts/game/HealthPickupController.h"

using namespace SFMLEngine;

/*
A health pickup that floats in the level, and can be collected by the player to gain health or score.
*/

class HealthPickup
{
public:
	// factory function
	static Entity Create(Scene* scene, const sf::Vector2f& position)
	{
		// create a new entity and give it a tag so it can be identified by the player
		Entity pickup = scene->CreateEntity();
		scene->SetEntityTag(pickup, "HealthPickup");

		// place the heart at its designated position
		scene->AddComponent(pickup, Transform{ position });
		// load its textures
		// the heart should be a lit sprite to make it fit in in the game world
		scene->AddComponent(pickup, SpriteRenderer{
				Texture::Create("assets/textures/heartPickup.png"),
				Material::Create("Lit"),
				0,
				Texture::Create("assets/textures/heartPickup_n.png")
			});

		// give the heart a collider
		// the heart itself does not occupy a whole 32x32 space
		// so give it a smaller collider and offset it
		BoxCollider collider{ { 16, 16 }, { 8, 8 } };
		// the heart should be a trigger; we do not want to resolve collisions between the player and hearts
		collider.SetTrigger(true);
		scene->AddComponent(pickup, collider);
		// let the collision system know it has a box collider
		scene->AddComponent(pickup, ColliderInfo{ ColliderType::Box });

		// give it a health pickup controller to control its animation and its lifetime
		scene->AddNativeScript<HealthPickupController>(pickup);

		// return the entity handle in case the caller wants to do more with the entity
		return pickup;
	}

};
