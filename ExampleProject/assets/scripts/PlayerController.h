#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

class PlayerController : public ScriptableEntity
{
public:
	void Start() override;
	void Update(Timestep ts) override;
	
	void OnCollisionEnter(const Collision& collision) override;
	void OnCollisionExit(Entity other) override;

	void Move(Timestep ts);
	void Jump(Timestep ts);

private:
	Transform* m_Transform = nullptr;
	Rigidbody* m_Rigidbody = nullptr;
	Animator* m_Animator = nullptr;

	bool m_OnGround = false;
	bool m_Attacking = false;

	float m_Friction = 14.0f;

	float m_MoveSpeed = 250.0f;
	float m_JumpPower = 450.0f;

	float m_FallMultiplier = 0.4f;

	bool m_FacingRight = true;
};
