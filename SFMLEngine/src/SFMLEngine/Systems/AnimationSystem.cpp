#include "AnimationSystem.h"


#include "SFMLEngine/ECS/Components/Animator.h"
#include "SFMLEngine/ECS/Components/SpriteRenderer.h"


namespace SFMLEngine {

	void AnimationSystem::Init(Coordinator* coordinator)
	{
		m_Coordinator = coordinator;
	}

	void AnimationSystem::EntityAddedToSystem(Entity entity)
	{
		auto& animator = m_Coordinator->GetComponent<Animator>(entity);
		
		animator.Reset();
		if (animator.CurrentAnimation == "null") return;

		auto& spriteRenderer = m_Coordinator->GetComponent<SpriteRenderer>(entity);
		auto& currentAnimation = animator.GetCurrentAnimation();

		auto rect{ currentAnimation.Flipped != animator.Flip ? FlipRect(*currentAnimation.CurrentFrame) : *currentAnimation.CurrentFrame };
		spriteRenderer.Sprite.setTextureRect(rect);
	}

	void AnimationSystem::EntityRemovedFromSystem(Entity entity)
	{

	}

	void AnimationSystem::Update(Timestep ts)
	{
		for (auto entity : m_Entities)
		{
			auto& animator = m_Coordinator->GetComponent<Animator>(entity);
			if (animator.CurrentAnimation == "null") continue;

			auto& spriteRenderer = m_Coordinator->GetComponent<SpriteRenderer>(entity);
			
			auto& currentAnimation = animator.GetCurrentAnimation();
			currentAnimation.Animate(ts);

			// flip the sprite if only 1 of the animation flip or animator flip is set
			// if both are set, its the same as flipping it twice or not at all
			auto rect{ currentAnimation.Flipped != animator.Flip ? FlipRect(*currentAnimation.CurrentFrame) : *currentAnimation.CurrentFrame };
			// set the rect
			spriteRenderer.Sprite.setTextureRect(rect);
		}
	}

	sf::IntRect AnimationSystem::FlipRect(const sf::IntRect& rect)
	{
		return { rect.left + rect.width, rect.top, -rect.width, rect.height };
	}

}
