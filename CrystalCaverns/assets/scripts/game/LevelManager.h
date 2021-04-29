#pragma once

#include <SFMLEngine.h>
#include "PlayerController.h"
#include "CameraController.h"
#include "Fader.h"


using namespace SFMLEngine;


class LevelManager : public ScriptableEntity
{
	enum class Action
	{
		None,
		LoadLevel1,
		ToDeathScreen,
		ToWinScreen
	};

public:
	void Start() override;
	void Update(float ts) override;

	void OnPlayerDeath();
	void OnTriggerEnter(const Collision& collision) override;

	void SetMainScene(Scene* s) { m_MainScene = s; }

private:

	void LoadLevel();
	void LoadDeathScreen();
	void LoadWinScreen();

private:
	Application* m_App = nullptr;
	Scene* m_MainScene = nullptr;

	Action m_Action = Action::None;

	bool* m_LoadedLevel1 = nullptr;

	bool m_FadingIn = false;
	bool m_FadingOut = false;

	PlayerController* m_PlayerController = nullptr;
	CameraController* m_CameraController = nullptr;


	Entity m_Fader = INVALID_ENTITY_ID;
	Fader* m_FaderScript = nullptr;

};
