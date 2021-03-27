#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

class EnemyController: public ScriptableEntity
{
public:
	void Start() override
	{
		m_Rigidbody = &GetComponent<Rigidbody>();
		m_Transform = &GetComponent<Transform>();
		m_Animator = &GetComponent<Animator>();

		m_PlayerTransform = &GetComponent<Transform>(GetEntitiesWithTag("Player")[0]);

		m_GroundLayerMask = LayerManager::GetLayer("Ground");
	}

	void Update(Timestep ts) override
	{
		float dist = Math::SquareMagnitude(m_PlayerTransform->Position - m_Transform->Position);

		if (m_Waking)
		{
			if (!m_Animator->GetCurrentAnimation().Playing)
			{
				// if the animation has stopped playing the wolf is awake
				m_Waking = false;
				m_Sleeping = false;
			}
		}
		
		
		if (fabsf(dist) > m_MaxPlayerFollowDistance * m_MaxPlayerFollowDistance)
		{
			m_Sleeping = true;
			m_Animator->SetCurrentAnimation("sleep");
			m_Rigidbody->Velocity.x = 0;
		}
		else if (m_Sleeping)
		{
			m_Waking = true;
			m_Animator->SetCurrentAnimation("wake");
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


			if (fabsf(m_Rigidbody->Velocity.x) > 50)
			{
				m_Animator->SetCurrentAnimation("walk");
			}
			else
			{
				m_Animator->SetCurrentAnimation("idle");
			}
		}

		m_Animator->Flip = !m_FacingRight;


		DEBUG_DISPLAY("Enemy position", m_Rigidbody->Position);
		DEBUG_DISPLAY("Enemy velocity", m_Rigidbody->Velocity);
		DEBUG_DISPLAY("Enemy facing right", m_FacingRight);
		DEBUG_DISPLAY("Enemy against wall", m_AgainstWall);
	}

private:
	Rigidbody* m_Rigidbody;
	Transform* m_Transform;
	Animator* m_Animator;

	Transform* m_PlayerTransform;
	
	Layer m_GroundLayerMask;

	bool m_FacingRight = true;
	bool m_AgainstWall = false;

	bool m_Sleeping = false;
	bool m_Waking = false;

	sf::Vector2f m_LeftCastPoint{ -0.5f, 3 };
	sf::Vector2f m_RightCastPoint{ 64, 3 };
	sf::Vector2f m_CastSize{ 0.5f, 29 };

	float m_MoveSpeed = 100.0f;
	float m_Friction = 8.0f;

	float m_MinPlayerFollowDistance = 20.0f;
	float m_MaxPlayerFollowDistance = 200.0f;

};
