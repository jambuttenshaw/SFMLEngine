#pragma once

#include <SFMLEngine.h>
#include "PlayerData.h"

using namespace SFMLEngine;


class PlayerStatsController : public ScriptableEntity
{
public:
	void Start() override;

	void Damage();
	bool IsDead() const { return m_Health == 0; }

	void SetupHearts(const std::vector<Entity>* const heartEntities);
	void UpdateHearts();


	void AssignPlayerData();

private:
	int m_MaxHealth = 0;
	int m_Health = m_MaxHealth;


	std::vector<Animator*> m_HeartAnimators;

};
