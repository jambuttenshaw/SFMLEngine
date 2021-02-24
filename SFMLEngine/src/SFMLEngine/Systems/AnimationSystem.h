#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/Coordinator.h"

#include "SFMLEngine/Timestep.h"


namespace SFMLEngine {


	class AnimationSystem : public System
	{
	public:
		void Init(Coordinator* coordinator);

		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override;

		void Update(Timestep ts);

	private:
		sf::IntRect FlipRect(const sf::IntRect& rect);

	private:
		Coordinator* m_Coordinator = nullptr;

	};

}