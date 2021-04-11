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

	void Update(Timestep ts) override
	{
		m_Transform->SetPosition(Input::GetMouseWorldPos());
	}


private:
	Transform* m_Transform = nullptr;
};
