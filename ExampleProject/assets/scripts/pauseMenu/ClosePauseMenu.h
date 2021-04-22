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
		if (m_Scene)
			Application::GetApplicationHandle()->DeleteScene(m_Scene);
	}

private:
	Scene* m_Scene = nullptr;

};
