#pragma once

#include <SFMLEngine.h>

#include "PlayerStatsController.h"

using namespace SFMLEngine;

class PlayerController : public ScriptableEntity
{
public:
	void Start() override;
	void Update(Timestep ts) override;
	
	void OnTriggerEnter(const Collision& collision) override;
	void OnTriggerStay(const Collision& collision) override;
	void OnTriggerExit(const std::pair<Entity, ColliderID>& other) override;

	void Move(Timestep ts);
	void Jump(Timestep ts);

	void StartCrawl();
	void EndCrawl();

	void Hurt(bool toTheRight);

	void Reset();

	sf::Vector2f GetCentre()
	{
		if (m_Crawling)
			return m_Transform->GetPosition() + sf::Vector2f{32, 16}; 
		else
			return m_Transform->GetPosition() + sf::Vector2f{16, 32};
	}

private:

	sf::Vector2f m_LeftCastPoint;
	sf::Vector2f m_RightCastPoint;
	sf::Vector2f m_BottomCastPoint;

	sf::Vector2f m_HorizontalCastSize;
	sf::Vector2f m_VerticalCastSize;

	Transform* m_Transform = nullptr;
	Rigidbody* m_Rigidbody = nullptr;
	Animator* m_Animator = nullptr;
	BoxCollider* m_Collider = nullptr;

	PlayerStatsController* m_StatsController = nullptr;

	Layer m_GroundLayerMask;

	bool m_OnGround = false;
	bool m_OnJumpThrough = false;
	int m_JumpThroughContactCount = 0;

	bool m_AgainstWall = false;
	bool m_CanLandOnPlatform = true;
	bool m_Crawling = false;

	int m_LadderContacts = 0;
	bool m_Sliding = false;
	float m_ClimbSpeed = 100.0f;
	float m_ClimbHorizontalFactor = 0.3f;

	bool m_Hurting = false;
	float m_HurtBounceVelocity = 175.0f;

	bool m_InJumpThroughPlatform = false;

	float m_Friction = 14.0f;

	float m_MoveSpeed = 250.0f;
	float m_JumpPower = 445.0f;

	float m_FallMultiplier = 0.4f;

	bool m_FacingRight = true;


	bool m_Dead = false;


	const sf::FloatRect m_StandingHitbox{ 8, 16, 17, 48 };
	const sf::FloatRect m_CrawlingHitbox{ 5, 10, 54, 22 };
};
