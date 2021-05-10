#pragma once

#include <SFMLEngine.h>
using namespace SFMLEngine;


class BackgroundMover : public ScriptableEntity
{
public:
	void Start() override
	{
		m_CameraTransform = &GetComponent<Transform>(GetEntitiesWithTag("MainCamera")[0]);
		m_Transform = &GetComponent<Transform>();
		m_Offset = m_Transform->GetPosition();
	}

	void Update(float ts) override
	{
		// work out its new position based off of the cameras current position
		sf::Vector2f pos{ m_MoveFactor * m_CameraTransform->GetPosition().x, m_CameraTransform->GetPosition().y };
		// place at its calculated position
		// incorporate the offset it was assigned initially
		m_Transform->SetPosition(pos + m_Offset);

		// work out if this tile has gone off screen
		// if it has then wrap it round to the other side 
		// we can work this out by transforming its position into the cameras local space
		pos = m_CameraTransform->GetWorldToLocalTransformMatrix().transformPoint(m_Transform->GetPosition());
		// if the right of the object is off the left of the screen
		if (pos.x + 256 <= -950)
		{
			m_Transform->Translate({ 256.0f * s_NumBackgroundTiles, 0 });
		}
		else if (pos.x >= 950)
		{
			m_Transform->Translate({ -256.0f * s_NumBackgroundTiles, 0 });
		}
	}

	void SetMoveFactor(float factor)
	{
		// the move factor is how much it follows the camera
		// the stronger the move factor the more it will follow the camera
		if (factor == 0)
			m_MoveFactor = 1;
		else
			m_MoveFactor = 1.0f / factor;
	}
	
	// a constant value saying how many background segments will be in existence
	// this is calculated to be the minimum required to fill the screen
	static inline const int s_NumBackgroundTiles = 7;

private:
	Transform* m_CameraTransform = nullptr;
	Transform* m_Transform = nullptr;
	float m_MoveFactor = 0.0f;
	sf::Vector2f m_Offset;
};


/*
The BackgroundScript controls all of the individual background segments to make it seamlessly loop
*/
class BackgroundScript : public ScriptableEntity
{
public:

	void Start() override
	{
		// load in the different textures
		ResourceID background1 = Texture::Create("assets/textures/background1.png");
		ResourceID background2 = Texture::Create("assets/textures/background2.png");
		ResourceID backgroundNormals = Texture::Create("assets/textures/background_n.png");

		// loop for the number of background tiles there should be
		for (int x = 0; x < BackgroundMover::s_NumBackgroundTiles; x++)
		{
			// create 2 new enities, one for each layer of the background
			Entity newBackground1 = CreateEntity();
			Entity newBackground2 = CreateEntity();

			// create a transform that positions the layer at its intial position
			Transform t;
			t.SetPosition({ (256.0f * (x - (BackgroundMover::s_NumBackgroundTiles / 2))) - 128.0f, -540.0f });
			AddComponent(newBackground1, t);
			AddComponent(newBackground2, t);

			// add a different texture for each layer
			AddComponent(newBackground1, SpriteRenderer{
				background2,
				Material::Create("Lit"),
				-1,
				 backgroundNormals });
			AddComponent(newBackground2, SpriteRenderer{
				background1,
				Material::Create("Lit"),
				-2,
				 backgroundNormals });

			// both layers need the mover script
			// but they have different move factors
			// which is what produces the cool parallax effect

			auto& script1 = AddNativeScript<BackgroundMover>(newBackground1);
			script1.SetMoveFactor(1.4f);

			auto& script2 = AddNativeScript<BackgroundMover>(newBackground2);
			script2.SetMoveFactor(1.2f);
		}
		
	}

};