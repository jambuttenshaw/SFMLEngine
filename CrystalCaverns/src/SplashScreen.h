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
		m_MainText = CreateEntity();
		AddComponent(m_MainText, Transform{ { -282, -66 } });
		AddComponent(m_MainText, SpriteRenderer{
			Texture::Create("assets/textures/splash/text.png"),
			Material::Create("Lit"),
			0,
			Texture::Create("assets/textures/splash/text_n.png"),
			});

		// create a camera since every scene needs a camera
		m_Camera = CreateEntity();
		SetEntityTag(m_Camera, "Camera");
		AddComponent(m_Camera, Transform{ });
		AddComponent(m_Camera, Camera{ });


		// create a light that will be animated to create a cool splash screen effect
		m_Light = CreateEntity();
		SetEntityTag(m_Light, "SceneLight");
		AddComponent(m_Light, Transform{ { 0, 10000 } });
		// both the point light and the directional light will be animated
		AddComponent(m_Light, PointLight{ 1.0f, 75.0f, sf::Color::White });
		AddComponent(m_Light, DirectionalLight{ 0.0f, -1.5f, 0.0f, sf::Color::White });
		// add the script that controls the animation
		AddNativeScript<SplashScreenAnimator>(m_Light);
	}

private:
	Entity m_MainText = INVALID_ENTITY_ID;
	Entity m_Light = INVALID_ENTITY_ID;
	Entity m_Camera = INVALID_ENTITY_ID;
};
