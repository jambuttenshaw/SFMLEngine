#include "AnimationSystem.h"


#include "SFMLEngine/ECS/Components/Animator.h"
#include "SFMLEngine/ECS/Components/SpriteRenderer.h"
#include "SFMLEngine/ECS/Components/GUI/GUIImage.h"


namespace SFMLEngine {

	void AnimationSystem::Init(Coordinator* coordinator)
	{
		m_Coordinator = coordinator;
	}

	void AnimationSystem::EntityAddedToSystem(Entity entity)
	{
		auto& animator = m_Coordinator->GetComponent<Animator>(entity);
		
		animator.Reset();
		if (animator.GetCurrentAnimationName() == "null") return;

		Animable* animObject = nullptr;
		switch (animator.GetAnimableType())
		{
		case AnimableType::Invalid:		SFMLE_CORE_ASSERT(0, "Invalid animable type!");						break;
		case AnimableType::Sprite:		animObject = &m_Coordinator->GetComponent<SpriteRenderer>(entity);	break;
		case AnimableType::GUIImage:	animObject = &m_Coordinator->GetComponent<GUIImage>(entity);		break;
		}
		auto& currentAnimation = animator.GetCurrentAnimation();

		animObject->SetFrame(*currentAnimation.GetCurrentFrame(), currentAnimation.IsFlipped() != animator.GetFlipped());
	}

	void AnimationSystem::EntityRemovedFromSystem(Entity entity)
	{

	}

	void AnimationSystem::Update(Timestep ts)
	{
		for (auto entity : m_Entities)
		{
			auto& animator = m_Coordinator->GetComponent<Animator>(entity);
			if (animator.GetCurrentAnimationName() == "null") continue;

			Animable* animObject = nullptr;
			switch (animator.GetAnimableType())
			{
			case AnimableType::Invalid:		SFMLE_CORE_ASSERT(0, "Invalid animable type!");						break;
			case AnimableType::Sprite:		animObject = &m_Coordinator->GetComponent<SpriteRenderer>(entity);	break;
			case AnimableType::GUIImage:	animObject = &m_Coordinator->GetComponent<GUIImage>(entity);		break;
			}
			auto& currentAnimation = animator.GetCurrentAnimation();
			

			currentAnimation.Animate(ts);


			animObject->SetFrame(*currentAnimation.GetCurrentFrame(), currentAnimation.IsFlipped() != animator.GetFlipped());
		}
	}
}
