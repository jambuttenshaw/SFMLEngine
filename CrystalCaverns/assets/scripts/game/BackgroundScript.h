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
	}

	void SetMoveFactor(float factor)
	{
		if (factor == 0)
			m_MoveFactor = 1;
		else
			m_MoveFactor = 1.0f / factor;
	}

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

		const int numBackgroundTiles = 7;
		for (int x = 0; x < numBackgroundTiles; x++)
		{
			Entity newBackground1 = CreateEntity();
			Entity newBackground2 = CreateEntity();

			Transform t;
			t.SetPosition({ (256.0f * (x - (numBackgroundTiles / 2))) - 128.0f, -540.0f });
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