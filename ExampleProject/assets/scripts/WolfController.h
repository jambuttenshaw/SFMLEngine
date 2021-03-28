#pragma once

#include <SFMLEngine.h>

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



		switch (m_State)
		{
		case WolfState::Sleep:
			// the wolf doesnt do anything when its sleeping
			break;



		case WolfState::Wake:
			// check to see if the wolf has woken up
			if (!m_Animator->GetCurrentAnimation().Playing)
			{
				// if the animation has stopped playing the wolf is awake
				m_State = WolfState::Alert;
				m_Animator->SetCurrentAnimation("alert");

				// the wolfs interest is reset when it wakes up
				m_Interest = m_InitialInterest;
			}
			break;



		case WolfState::Alert:
			// check if the player is close enough to follow
			if (fabsf(dist) > m_MaxPlayerFollowDistance * m_MaxPlayerFollowDistance)
			{
				// the player is too far away
				// the wolf should start to lose interest
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
				// the player is close; the wolf should follow it
				m_State = WolfState::Follow;
				// the wolf becomes interested
				m_Interest = m_InitialInterest;
			}
			break;



		case WolfState::Follow:
			// the wolf is following the player
			// first check if the player has moved out of range
			if (fabsf(dist) > m_MaxPlayerFollowDistance * m_MaxPlayerFollowDistance)
			{
				// the player is now too far away, so the wolf should go on alert instead
				m_State = WolfState::Alert;
				m_Animator->SetCurrentAnimation("alert");
				m_Rigidbody->Velocity.x = 0;

			}
			else
			{
				// the player is close; the wolf should move towards it
				float diff = m_PlayerTransform->Position.x - m_Transform->Position.x - 16;
				m_FacingRight = diff > 0;

				m_AgainstWall = Physics::BoxCast({ m_Transform->Position + (m_FacingRight ? m_RightCastPoint : m_LeftCastPoint), m_CastSize }, m_GroundLayerMask).first;

				m_Rigidbody->Velocity.x = Math::Lerp(m_Rigidbody->Velocity.x, 0.0f, m_Friction * ts);
				if (!m_AgainstWall)
				{
					if (fabsf(diff) > m_MinPlayerFollowDistance)
					{
						// ts is not a factor in the velocity
						// as it is not a change of velocity over time; it is being set to a constant value
						// then the delta time will be taken into account when the velocity is applied in the physics system
						m_Rigidbody->Velocity.x = m_MoveSpeed * (m_FacingRight ? 1 : -1);

						// set the animation of the wolf according to how fast its moving
						if (fabsf(m_Rigidbody->Velocity.x) > 100)
						{
							m_Animator->SetCurrentAnimation("run");
						}
						else if (fabsf(m_Rigidbody->Velocity.x) > 50)
						{
							m_Animator->SetCurrentAnimation("walk");
						}
					}
					else
					{
						// the player is really close; within biting range!
						m_State = WolfState::Attack;
						m_Animator->SetCurrentAnimation("bite");
						m_Animator->Reset();

						m_Rigidbody->Velocity.x = 0;
					}
				}
				else
				{
					m_Animator->SetCurrentAnimation("idle");
				}
			}
			break;
			


		case WolfState::Attack:
			// the wolf should be attacking the player
			if (!m_Animator->GetCurrentAnimation().Playing)
			{
				// if the animation has stopped playing the wolf has finished biting
				// we go back to chasing the player
				m_State = WolfState::Follow;
			}
			break;
		}



		m_Animator->Flip = !m_FacingRight;
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
