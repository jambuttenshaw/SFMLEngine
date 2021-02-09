#pragma once


#include "../ECS/Coordinator.h"
#include "../ECS/Components/Rigidbody.h"
#include "../ECS/Components/Transform.h"

#include "../Timestep.h"


namespace SFMLEngine {

	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem() : m_Gravity(0, 100) {}
		~PhysicsSystem() = default;

		void Init(Coordinator* coordinator);

		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override;

		void Update(Timestep ts);

	private:
		Coordinator* m_Coordinator = nullptr;

		sf::Vector2f m_Gravity;
	};

}
