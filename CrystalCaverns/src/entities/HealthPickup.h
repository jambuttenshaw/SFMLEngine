#pragma once

#include <SFMLEngine.h>
#include "scripts/game/HealthPickupController.h"

using namespace SFMLEngine;


class HealthPickup
{
public:
	static Entity Create(Scene* scene, const sf::Vector2f& position)
	{
		Entity pickup = scene->CreateEntity();
		scene->SetEntityTag(pickup, "HealthPickup");

		scene->AddComponent(pickup, Transform{ position });
		scene->AddComponent(pickup, SpriteRenderer{
				Texture::Create("assets/textures/heartPickup.png"),
				Material::Create("Lit"),
				0,
				Texture::Create("assets/textures/heartPickup_n.png")
			});

		BoxCollider collider{ { 16, 16 }, { 8, 8 } };
		collider.SetTrigger(true);
		scene->AddComponent(pickup, collider);
		scene->AddComponent(pickup, ColliderInfo{ ColliderType::Box });

		scene->AddNativeScript<HealthPickupController>(pickup);

		return pickup;
	}

};
