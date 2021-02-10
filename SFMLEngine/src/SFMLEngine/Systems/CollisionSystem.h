#pragma once


#include "../ECS/Coordinator.h"
#include "../ECS/Components/Colliders/ICollider.h"


namespace SFMLEngine {

	struct Collision
	{
		bool Collided;
		Entity Other;
	};

	class CollisionSystem : public System
	{
		CollisionSystem() = default;
		~CollisionSystem() = default;

		void Init(Coordinator* coordinator);


		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override;


		const Collision TestCollision(const ICollider& collider);

	private:
		template<typename T>
		const Collision DoCollisionTest(const T& collider)
		{
			for (auto& entity : m_Entities)
			{
				ICollider& other = m_Coordinator->GetComponent<ICollider>(entity);
				
				if (other.Colliding(collider))
				{
					return Collision{ true, entity };
				}
			}
			return Collision{ false, NULL_ENTITY_ID };
		}

	private:
		Coordinator* m_Coordinator = nullptr;
	};

}
