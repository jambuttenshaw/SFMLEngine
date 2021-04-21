#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/Coordinator.h"


namespace SFMLEngine {


	class AnimationSystem : public System
	{
	public:
		void Init(Coordinator* coordinator);

		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override;

		void Update(float ts);

	private:
		Coordinator* m_Coordinator = nullptr;

	};

}