#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;


class PlayerHeart
{
public:
	static Entity Create(Scene* scene, const sf::Vector2f& position)
	{
		Entity newHeart = scene->CreateEntity();


		scene->AddComponent(newHeart, GUITransform{ position, GUIElementType::Image });
		scene->AddComponent(newHeart, GUIImage{ Texture::Create("assets/textures/heart.png"), 0 });


		Animation full{ "full", {
			{ 0, 0,   64, 64 } },
			1.0f
		}; full.SetLooping(false);
		Animation half{ "half", {
			{ 64, 0,   64, 64 } },
			1.0f
		}; half.SetLooping(false);
		Animation empty{ "empty", {
			{ 128, 0,   64, 64 } },
			1.0f
		}; empty.SetLooping(false);

		Animator animator{ AnimableType::GUIImage };
		animator.AddAnimation(full);
		animator.AddAnimation(half);
		animator.AddAnimation(empty);

		animator.SetCurrentAnimation("full");
		scene->AddComponent(newHeart, animator);


		return newHeart;
	}
	
};
