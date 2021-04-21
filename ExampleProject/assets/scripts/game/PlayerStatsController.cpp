#include "PlayerStatsController.h"
#include "LevelManager.h"



void PlayerStatsController::Start()
{
	m_Health = m_MaxHealth;
}


void PlayerStatsController::Damage()
{
	m_Health -= 1;
	if (m_Health <= 0)
	{
		m_Health = 0;

		// player has died!
		// move to the death screen
		GetNativeScript<LevelManager>().OnPlayerDeath();

	}

	UpdateHearts();
}


void PlayerStatsController::SetupHearts(const std::vector<Entity>* const heartEntities)
{
	for (auto const& heart : *heartEntities)
	{
		m_HeartAnimators.push_back(&GetComponent<Animator>(heart));
	}

	// there is 2 health points for each heart
	m_MaxHealth = static_cast<int>(m_HeartAnimators.size() * 2);
	m_Health = m_MaxHealth;
}

void PlayerStatsController::UpdateHearts()
{
	int fullValue = 2;
	for (auto& animator : m_HeartAnimators)
	{
		if (m_Health >= fullValue)
		{
			// this heart is full
			animator->SetCurrentAnimation("full");
		}
		else if (m_Health + 1 == fullValue)
		{
			// this heart is half-full
			animator->SetCurrentAnimation("half");
		}
		else
		{
			// this heart is empty
			animator->SetCurrentAnimation("empty");
		}
		fullValue += 2;
	}
}
