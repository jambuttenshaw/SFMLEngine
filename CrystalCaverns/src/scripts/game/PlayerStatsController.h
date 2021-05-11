#pragma once

#include <SFMLEngine.h>
#include "PlayerData.h"

using namespace SFMLEngine;


class CrystalCollector;

/*
PlayerStatsController is in charge of managing the players health points
and the hearts GUI to display how much health it has
*/
class PlayerStatsController : public ScriptableEntity
{
public:
	void Start() override;

	void OnTriggerEnter(const Collision& collision) override;

	void Damage();
	bool IsDead() const { return m_Health == 0; }

	void SetupHearts(const std::vector<Entity>* const heartEntities);
	void UpdateHearts();


	void AssignPlayerData();

private:
	int m_MaxHealth = 0;
	int m_Health = m_MaxHealth;

	CrystalCollector* m_CrystalCollector = nullptr;


	std::vector<Animator*> m_HeartAnimators;

};
