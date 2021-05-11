#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;


class GoToMouse : public ScriptableEntity
{
public:
	void Start() override
	{
		m_Transform = &GetComponent<Transform>();
	}

	void Update(float ts) override
	{
		// simply set the position of this sprite to the world position of the mouse
		m_Transform->SetPosition(Input::GetMouseWorldPos());
	}


private:
	Transform* m_Transform = nullptr;
};
