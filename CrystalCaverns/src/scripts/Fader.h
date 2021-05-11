#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;



class Fader : public ScriptableEntity
{
public:

	// the states that a fader can be in
	enum class State
	{
		Still,
		FadeIn,
		FadeOut
	};

	void Start() override
	{
		// set up the fader
		// put it in a really high render layer so it will appear on top of everything else
		AddComponent(GUIImage{ Texture::Create("assets/textures/black.png"), 99 });

		// scale it so that it always takes up the entire screen
		// no matter what the window size is
		GUITransform t{ {0.0f, 0.0f }, GUIElementType::Image };
		sf::Vector2f windowSize = static_cast<sf::Vector2f>(Application::GetWindowSize());
		t.SetScale({ windowSize.x / 256.0f, windowSize.y / 256.0f });
		AddComponent(t);

		// set the colour of the image to fully transparent initially
		m_Image = &GetComponent<GUIImage>();
		m_Image->SetColor({ 255, 255, 255, 0 });
	}

	void Update(float ts) override
	{
		switch (m_State)
		{
		case State::FadeIn:
			// the fader is fading into existence
			// increment the timer
			m_FadeProgress += ts * m_FadeSpeed;
			if (m_FadeProgress >= 1)
			{
				// the image has fully faded in
				// stop fading
				m_FadeProgress = 1;
				m_State = State::Still;

			}
			else
			{
				// gradually make it more opaque
				m_Image->SetColor({ 255, 255, 255, sf::Uint8(m_FadeProgress * 255) });
			}

			break;
		case State::FadeOut:
			// it is fading out
			// increase timer
			m_FadeProgress += ts * m_FadeSpeed;
			if (m_FadeProgress >= 1)
			{
				// the image has fully faded out
				m_FadeProgress = 1;
				// go back to doing nothing
				m_State = State::Still;
			}
			else
			{
				// gradually make the image more transparent
				m_Image->SetColor({ 255, 255, 255, sf::Uint8((1 - m_FadeProgress) * 255) });
			}

			break;

		case State::Still: break; // do nothing
		}
	}

	void SetFadeState(State state)
	{
		// set the fader to either fading in or out
		m_State = state;
		m_FadeProgress = 0;
	}

	// check if its finished fading in or out
	bool FadeComplete() { return m_FadeProgress >= 1 && m_State == State::Still; }
	// set the speed at which it fades
	void SetFadeSpeed(float speed) { m_FadeSpeed = speed; }

private:
	State m_State = State::Still;
	float m_FadeProgress = 0.0f;
	float m_FadeSpeed = 3.0f;

	GUIImage* m_Image = nullptr;
};
