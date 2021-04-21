#pragma once

#include <SFMLEngine.h>

#include "PlayerController.h"

using namespace SFMLEngine;

class WolfController: public ScriptableEntity
{
public:

	enum class WolfState
	{
		Sleep,
		Wake,
		Alert,
		Follow,
		Attack,
		Climb
	};

public:
	void Start() override;

	void Update(float ts) override;

	void Wake();

	void StartClimb();
	void EndClimb();
	void EndClimbFromRoof();

	sf::Vector2f GetCentre();

private:
	Rigidbody* m_Rigidbody;
	Transform* m_Transform;
	Animator* m_Animator;
	BoxCollider* m_Collider;

	PlayerController* m_PlayerController;
	
	Layer m_GroundLayerMask;

	bool m_FacingRight = true;
	bool m_AgainstWall = false;

	WolfState m_State = WolfState::Sleep;

	float m_InitialInterest = 5.0f;
	float m_Interest = m_InitialInterest;

	sf::FloatRect m_StandHitbox{0, 3, 64, 29};
	sf::FloatRect m_ClimbHitbox{3, 0, 29, 64};

	sf::Vector2f m_LeftCastPoint{ -0.5f, 3 };
	sf::Vector2f m_RightCastPoint{ 64, 3 };
	sf::Vector2f m_CastSize{ 0.5f, 29 };
	
	sf::Vector2f m_ClimbLeftCastPoint{ -0.5f, 0 };
	sf::Vector2f m_ClimbRightCastPoint{ 32, 0 };
	sf::Vector2f m_ClimbCastSize{ 0.5f, 32 };

	sf::Vector2f m_LeftCanClimbCastPoint{ 4, -32.5f };
	sf::Vector2f m_RightCanClimbCastPoint{ 44, -32.5f };

	sf::Vector2f m_ClimbRoofCastPoint{ 8, -0.5f };
	sf::Vector2f m_ClimbRoofCastSize{ 16, 5.0f };



	float m_MoveSpeed = 240.0f;
	float m_ClimbSpeed = 100.0f;
	float m_Friction = 8.0f;

	float m_AttackDist = 27.0f;
	float m_MinPlayerFollowDistance = 20.0f;
	float m_MaxPlayerFollowDistance = 300.0f;

	float m_InitialAttackCooldown = 1.0f;
	float m_AttackCooldown = 0.0f;

};
