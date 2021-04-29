#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;


#include "splash/SplashScreenAnimator.h"


class SplashScreen : public Scene
{
public:

	void Create() override
	{
		m_MainText = CreateEntity();
		AddComponent(m_MainText, Transform{ { -282, -66 } });
		AddComponent(m_MainText, SpriteRenderer{
			Texture::Create("assets/textures/splash/text.png"),
			Material::Create("Lit"),
			0,
			Texture::Create("assets/textures/splash/text_n.png"),
			});

		m_Camera = CreateEntity();
		SetEntityTag(m_Camera, "Camera");

		AddComponent(m_Camera, Transform{ });
		AddComponent(m_Camera, Camera{ });



		m_Light = CreateEntity();
		SetEntityTag(m_Light, "SceneLight");
		AddComponent(m_Light, Transform{ { 0, 10000 } });
		AddComponent(m_Light, PointLight{ 1.0f, 75.0f, sf::Color::White });
		AddComponent(m_Light, DirectionalLight{ 0.0f, -1.5f, 0.0f, sf::Color::White });
		AddNativeScript<SplashScreenAnimator>(m_Light);
	}

private:
	Entity m_MainText = INVALID_ENTITY_ID;
	Entity m_Light = INVALID_ENTITY_ID;
	Entity m_Camera = INVALID_ENTITY_ID;
};
