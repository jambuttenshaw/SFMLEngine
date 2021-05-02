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
		sf::Vector2f pos{ m_MoveFactor * m_CameraTransform->GetPosition().x, m_CameraTransform->GetPosition().y };
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
		if (factor == 0)
			m_MoveFactor = 1;
		else
			m_MoveFactor = 1.0f / factor;
	}
	
	static inline const int s_NumBackgroundTiles = 7;

private:
	Transform* m_CameraTransform = nullptr;
	Transform* m_Transform = nullptr;
	float m_MoveFactor = 0.0f;
	sf::Vector2f m_Offset;
};


class BackgroundScript : public ScriptableEntity
{
public:

	void Start() override
	{
		ResourceID background1 = Texture::Create("assets/textures/background1.png");
		ResourceID background2 = Texture::Create("assets/textures/background2.png");
		ResourceID backgroundNormals = Texture::Create("assets/textures/background_n.png");

		for (int x = 0; x < BackgroundMover::s_NumBackgroundTiles; x++)
		{
			Entity newBackground1 = CreateEntity();
			Entity newBackground2 = CreateEntity();

			Transform t;
			t.SetPosition({ (256.0f * (x - (BackgroundMover::s_NumBackgroundTiles / 2))) - 128.0f, -540.0f });
			AddComponent(newBackground1, t);
			AddComponent(newBackground2, t);

			AddComponent(newBackground1, SpriteRenderer{
				background1,
				Material::Create("Lit"),
				-1,
				 backgroundNormals });
			AddComponent(newBackground2, SpriteRenderer{
				background2,
				Material::Create("Lit"),
				-2,
				 backgroundNormals });


			auto& script1 = AddNativeScript<BackgroundMover>(newBackground1);
			script1.SetMoveFactor(1.4f);

			auto& script2 = AddNativeScript<BackgroundMover>(newBackground2);
			script2.SetMoveFactor(1.2f);
		}
		
	}

};