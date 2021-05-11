#include "PlayerStatsController.h"
#include "LevelManager.h"

#include "CrystalCollector.h"
#include "HealthPickupController.h"
#include "Constants.h"

void PlayerStatsController::Start()
{
	m_Health = m_MaxHealth;

	m_CrystalCollector = &GetNativeScript<CrystalCollector>();

	// load sounds that are played when the player hurts/dies
	// they are relative to the listener as they always come from the player
	AudioSystem::LoadSound("hurt", "assets/audio/hurt.ogg", 50);
	AudioSystem::SetRelativeToListener("hurt", true);
	AudioSystem::LoadSound("die", "assets/audio/die.ogg", 50);
	AudioSystem::SetRelativeToListener("die", true);
}

void PlayerStatsController::OnTriggerEnter(const Collision& collision)
{
	// check if weve collided with a health pickup
	if (GetEntityTag(collision.Other) == "HealthPickup")
	{
		// collided with a health pickup

		// notify the script the heart has been collected and should be removed from the scene
		auto& controller = GetNativeScript<HealthPickupController>(collision.Other);
		controller.CollectHeart();

		// if we are missing hearts
		if (m_Health < m_MaxHealth)
		{
			// increase health and update the gui
			m_Health++;
			UpdateHearts();
		}
		else
		{
			// our hearts are full: the health pickup contributes to score instead
			m_CrystalCollector->ScorePoints(HEART_BONUS_POINTS);
		}
	}
}


void PlayerStatsController::Damage()
{
	// decrement health
	m_Health--;

	// check if the player has passed
	if (m_Health <= 0)
	{
		// clamp health at 0
		m_Health = 0;

		// player has died!
		AudioSystem::PlaySound("die");

		// move to the death screen
		GetNativeScript<LevelManager>().OnPlayerDeath();

	}
	else
	{
		// play ouch sound
		AudioSystem::PlaySound("hurt");
	}

	// and update the gui
	UpdateHearts();
}


void PlayerStatsController::SetupHearts(const std::vector<Entity>* const heartEntities)
{
	// we need to have a way to access the animators of all the hearts on the gui
	// we store them in a vector
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
	// fullValue is the amount of health that is required for this heart to be displayed as full
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

void PlayerStatsController::AssignPlayerData()
{
	// the health of the player is used in the win screen
	// so it is saved in the data store for access across scenes
	int* health = &DataStore::RetrieveData<PlayerData>("playerData")->Health;
	*health = m_Health;
}
