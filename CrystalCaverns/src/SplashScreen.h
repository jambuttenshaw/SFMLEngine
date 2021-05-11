#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;


#include "splash/SplashScreenAnimator.h"


/*
The splash screen just tells the user that I made the game with a neat little animation
and then fades out into the main menu
*/


class SplashScreen : public Scene
{
public:

	void Create() override
	{
		// populate the scene

		// create the big text
		// use a lit material so we can use lighting effects
		Entity mainText = CreateEntity();
		AddComponent(mainText, Transform{ { -282, -66 } });
		AddComponent(mainText, SpriteRenderer{
			Texture::Create("assets/textures/splash/text.png"),
			Material::Create("Lit"),
			0,
			Texture::Create("assets/textures/splash/text_n.png"),
			});

		// create a camera since every scene needs a camera
		Entity camera = CreateEntity();
		SetEntityTag(camera, "Camera");
		AddComponent(camera, Transform{ });
		AddComponent(camera, Camera{ });


		// create a light that will be animated to create a cool splash screen effect
		Entity light = CreateEntity();
		SetEntityTag(light, "SceneLight");
		AddComponent(light, Transform{ { 0, 10000 } });
		// both the point light and the directional light will be animated
		AddComponent(light, PointLight{ 1.0f, 75.0f, sf::Color::White });
		AddComponent(light, DirectionalLight{ 0.0f, -1.5f, 0.0f, sf::Color::White });
		// add the script that controls the animation
		AddNativeScript<SplashScreenAnimator>(light);
	}

};
