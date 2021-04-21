#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;



class Fader : public ScriptableEntity
{
public:

	enum class State
	{
		Still,
		FadeIn,
		FadeOut
	};

	void Start() override
	{
		AddComponent(GUIImage{ Texture::Create("assets/textures/black.png") });

		GUITransform t{ {0.0f, 0.0f }, GUIElementType::Image };
		sf::Vector2f windowSize = static_cast<sf::Vector2f>(Application::GetWindowSize());
		t.SetScale({ windowSize.x / 256.0f, windowSize.y / 256.0f });
		AddComponent(t);

		m_Image = &GetComponent<GUIImage>();
		m_Image->SetColor({ 255, 255, 255, 0 });
	}

	void Update(Timestep ts) override
	{
		switch (m_State)
		{
		case State::FadeIn:

			m_FadeProgress += ts * m_FadeSpeed;
			if (m_FadeProgress >= 1)
			{
				// the image has fully faded in
				m_FadeProgress = 1;
				m_State = State::Still;

			}
			else
			{
				m_Image->SetColor({ 255, 255, 255, sf::Uint8(m_FadeProgress * 255) });
			}

			break;
		case State::FadeOut:

			m_FadeProgress += ts * m_FadeSpeed;
			if (m_FadeProgress >= 1)
			{
				// the image has fully faded out
				m_FadeProgress = 1;
				m_State = State::Still;
			}
			else
			{
				m_Image->SetColor({ 255, 255, 255, sf::Uint8((1 - m_FadeProgress) * 255) });
			}

			break;

		case State::Still: break;
		}
	}

	void SetFadeState(State state)
	{
		m_State = state;
		m_FadeProgress = 0;
	}

	bool FadeComplete() { return m_FadeProgress >= 1 && m_State == State::Still; }
	void SetFadeSpeed(float speed) { m_FadeSpeed = speed; }

private:
	State m_State = State::Still;
	float m_FadeProgress = 0.0f;
	float m_FadeSpeed = 3.0f;

	GUIImage* m_Image = nullptr;
};
