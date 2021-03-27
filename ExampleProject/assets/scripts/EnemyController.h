#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

class EnemyController: public ScriptableEntity
{
public:

	enum class WolfState
	{
		Sleep,
		Wake,
		Alert,
		Follow,
		Attack
	};

public:
	void Start() override
	{
		m_Rigidbody = &GetComponent<Rigidbody>();
		m_Transform = &GetComponent<Transform>();
		
		m_Animator = &GetComponent<Animator>();
		m_Animator->SetCurrentAnimation("sleep");

		m_PlayerTransform = &GetComponent<Transform>(GetEntitiesWithTag("Player")[0]);

		m_GroundLayerMask = LayerManager::GetLayer("Ground");
	}

	void Update(Timestep ts) override
	{
		float dist = Math::SquareMagnitude(m_PlayerTransform->Position - m_Transform->Position);

		if (Input::IsKeyPressed(sf::Keyboard::R)) Wake();

		// waking up
		if (m_State == WolfState::Sleep)
		{
			// what will wake the wolf?
		}
		else if (m_State == WolfState::Wake)
		{
			if (!m_Animator->GetCurrentAnimation().Playing)
			{
				// if the animation has stopped playing the wolf is awake
				m_State = WolfState::Alert;
			}
		}
		else
		{
			// losing the player
			if (fabsf(dist) > m_MaxPlayerFollowDistance * m_MaxPlayerFollowDistance) // the player is too far to follow
			{
				if (m_State == WolfState::Alert)
				{
					m_Interest -= ts;
					if (m_Interest <= 0)
					{
						// the wolf has lost interest and is going to sleep
						m_State = WolfState::Sleep;
						m_Animator->SetCurrentAnimation("sleep");
					}
				}
				else
				{
					m_State = WolfState::Alert;
					m_Animator->SetCurrentAnimation("alert");
					m_Rigidbody->Velocity.x = 0;

					m_Interest = m_InitialInterest;
				}
			}
			else if (m_State == WolfState::Alert)
				// the player is close enough to follow but we are still in alert mode
				// play the un alert animation
			{
				m_State = WolfState::Follow;
			}
			else
			{
				float diff = m_PlayerTransform->Position.x - m_Transform->Position.x - 16;
				m_FacingRight = diff > 0;

				m_AgainstWall = Physics::BoxCast({ m_Transform->Position + (m_FacingRight ? m_RightCastPoint : m_LeftCastPoint), m_CastSize }, m_GroundLayerMask).first;

				m_Rigidbody->Velocity.x = Math::Lerp(m_Rigidbody->Velocity.x, 0.0f, m_Friction * ts);
				if (!m_AgainstWall && (fabsf(diff) > m_MinPlayerFollowDistance))
				{
					// ts is not a factor in the velocity
					// as it is not a change of velocity over time; it is being set to a constant value
					// then the delta time will be taken into account when the velocity is applied in the physics system
					m_Rigidbody->Velocity.x = m_MoveSpeed * (m_FacingRight ? 1 : -1);
				}


				if (fabsf(m_Rigidbody->Velocity.x) > 100)
				{
					m_Animator->SetCurrentAnimation("run");
				}
				else if (fabsf(m_Rigidbody->Velocity.x) > 50)
				{
					m_Animator->SetCurrentAnimation("walk");
				}
				else
				{
					m_Animator->SetCurrentAnimation("idle");
				}
			}
		}


		m_Animator->Flip = !m_FacingRight;


		DEBUG_DISPLAY("Enemy position", m_Rigidbody->Position);
		DEBUG_DISPLAY("Enemy velocity", m_Rigidbody->Velocity);
		DEBUG_DISPLAY("Enemy facing right", m_FacingRight);
		DEBUG_DISPLAY("Enemy against wall", m_AgainstWall);


	}


	void Wake()
	{
		if (m_State == WolfState::Sleep)
		{
			m_State = WolfState::Wake;
			m_Animator->SetCurrentAnimation("wake");
		}
	}

private:
	Rigidbody* m_Rigidbody;
	Transform* m_Transform;
	Animator* m_Animator;

	Transform* m_PlayerTransform;
	
	Layer m_GroundLayerMask;

	bool m_FacingRight = true;
	bool m_AgainstWall = false;

	WolfState m_State = WolfState::Sleep;

	float m_InitialInterest = 5.0f;
	float m_Interest = m_InitialInterest;

	sf::Vector2f m_LeftCastPoint{ -0.5f, 3 };
	sf::Vector2f m_RightCastPoint{ 64, 3 };
	sf::Vector2f m_CastSize{ 0.5f, 29 };

	float m_MoveSpeed = 200.0f;
	float m_Friction = 8.0f;

	float m_MinPlayerFollowDistance = 20.0f;
	float m_MaxPlayerFollowDistance = 200.0f;

};
