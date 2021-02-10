#pragma once

#include <SFML/Graphics.hpp>

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
	public:
		CollisionSystem() = default;
		~CollisionSystem() = default;

		void Init(Coordinator* coordinator);


		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override;


		const Collision TestCollision(Entity entity);

	private:
		template<typename T>
		const Collision DoCollisionTest(const T& collider, const sf::Vector2f& position, Entity& thisEntity)
		{
			for (auto& entity : m_Entities)
			{
				if (entity == thisEntity) continue;

				Collider& other = m_Coordinator->GetComponent<Collider>(entity);
				Transform& otherTransform = m_Coordinator->GetComponent<Transform>(entity);
				sf::Vector2f diff = position - otherTransform.Position;
				
				bool collision = false;
				switch (other.Type)
				{
				case ColliderType::Invalid:	SFMLE_CORE_ASSERT(0, "Invalid collider type!"); break;
				case ColliderType::Box:		collision =  m_Coordinator->GetComponent<BoxCollider>(entity).Colliding(collider, diff); break;
				case ColliderType::Circle:	collision = m_Coordinator->GetComponent<CircleCollider>(entity).Colliding(collider, diff); break;
				default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
				}

				if (collision)
				{
					return Collision{ true, entity };
				}
			}
			return Collision{ false, INVALID_ENTITY_ID };
		}

	private:
		Coordinator* m_Coordinator = nullptr;
	};

}
