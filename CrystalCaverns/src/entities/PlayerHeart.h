#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

/*
An image in the GUI used to display how much health the player has left
*/
class PlayerHeart
{
public:
	// factory function
	static Entity Create(Scene* scene, const sf::Vector2f& position)
	{
		// create a new entity
		Entity newHeart = scene->CreateEntity();

		// add a GUI transform, and place it at the specified position
		scene->AddComponent(newHeart, GUITransform{ position, GUIElementType::Image });
		// add the texture to the entity
		scene->AddComponent(newHeart, GUIImage{ Texture::Create("assets/textures/heart.png"), 0 });

		// the heart can either be full, half full, or empty
		// a single-frame animation is used to define each state
		// this is the easiest way for scripts to deal with changing the image of the hearts
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

		// create the animator component and add the animations to it
		Animator animator{ AnimableType::GUIImage };
		animator.AddAnimation(full);
		animator.AddAnimation(half);
		animator.AddAnimation(empty);

		// hearts should be full by default
		animator.SetCurrentAnimation("full");
		scene->AddComponent(newHeart, animator);

		// return the entity handle if the caller would like to do further processing with it
		return newHeart;
	}
	
};
