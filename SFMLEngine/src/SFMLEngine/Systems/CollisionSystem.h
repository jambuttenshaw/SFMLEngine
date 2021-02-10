#pragma once


#include "../ECS/Coordinator.h"
#include "../ECS/Components/Colliders/Collider.h"


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


		const Collision TestCollision(Entity entity);

	private:
		template<typename T>
		const Collision DoCollisionTest(const T& collider)
		{
			for (auto& entity : m_Entities)
			{
				Collider& other = m_Coordinator->GetComponent<Collider>(entity);
				
				bool collision = false;
				switch (other.Type)
				{
				case ColliderType::Box:		collision =  m_Coordinator->GetComponent<BoxCollider>(entity).Colliding(collider); break;
				case ColliderType::Circle:	collision = m_Coordinator->GetComponent<CircleCollider>(entity).Colliding(collider); break;
				default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
				}

				if (collision)
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
