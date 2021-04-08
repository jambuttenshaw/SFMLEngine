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


	void Damage()
	{
		m_Health -= 1;
		if (m_Health < 0) m_Health = 0;

		UpdateHearts();
	}


	void SetupHearts(const std::vector<Entity>* const heartEntities)
	{
		for (auto const& heart : *heartEntities)
		{
			m_HeartAnimators.push_back(&GetComponent<Animator>(heart));
		}

		// there is 2 health points for each heart
		m_MaxHealth = static_cast<int>(m_HeartAnimators.size() * 2);
		m_Health = m_MaxHealth;
	}

	void UpdateHearts()
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

private:
	int m_MaxHealth = 0;
	int m_Health = m_MaxHealth;


	std::vector<Animator*> m_HeartAnimators;

};
