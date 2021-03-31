#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

class PlayerStatsController : public ScriptableEntity
{
public:
	void Start() override
	{
		m_Health = m_MaxHealth;
	}

	void Update(Timestep ts) override
	{

	}


	void Damage(float amount)
	{
		m_Health -= amount;
		if (m_Health < 0) m_Health = 0;
	}


private:
	float m_MaxHealth = 100;
	float m_Health = m_MaxHealth;
};
