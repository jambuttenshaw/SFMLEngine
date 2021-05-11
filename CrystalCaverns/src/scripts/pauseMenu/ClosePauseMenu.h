#pragma once

#include <SFMLEngine.h>
using namespace SFMLEngine;


class ClosePauseMenu : public ScriptableEntity
{
public:

	void SetScene(Scene* scene)
	{
		m_Scene = scene;
	}

	void Close()
	{
		// deletes the scene that contains this entity
		// used for deleting the pause menu once the game is resumed
		if (m_Scene)
			Application::GetApplicationHandle()->DeleteScene(m_Scene);
	}

private:
	Scene* m_Scene = nullptr;

};
