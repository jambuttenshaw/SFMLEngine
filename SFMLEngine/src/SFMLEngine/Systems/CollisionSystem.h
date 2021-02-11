#pragma once

#include <SFML/Graphics.hpp>

#include "../ECS/Coordinator.h"
#include "../ECS/Components/Colliders/Collider.h"


namespace SFMLEngine {

	struct Collision
	{
		bool Collided;
		Entity Other;
		sf::FloatRect Bounds; // the boundaries of this objects collider
		sf::FloatRect OtherBounds; // the boundaries of the object the collision occurred with
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
		const Collision DoCollisionTest(T& collider, const sf::Vector2f& position, Entity& thisEntity)
		{
			ZoneScoped;
			for (auto& entity : m_Entities)
			{
				ZoneScoped;
				ZoneName("ProcessEntity", 13);

				if (entity == thisEntity) continue;

				Collider& other = m_Coordinator->GetComponent<Collider>(entity);
				Transform& otherTransform = m_Coordinator->GetComponent<Transform>(entity);
				sf::Vector2f diff = position - otherTransform.Position;
				
				CollisionData collision;
				switch (other.Type)
				{
				case ColliderType::Invalid:	SFMLE_CORE_ASSERT(0, "Invalid collider type!"); break;
				case ColliderType::Box:		collision =  m_Coordinator->GetComponent<BoxCollider>(entity).Colliding(collider, diff); break;
				case ColliderType::Circle:	collision = m_Coordinator->GetComponent<CircleCollider>(entity).Colliding(collider, diff); break;
				case ColliderType::Tilemap: collision = m_Coordinator->GetComponent<TilemapCollider>(entity).Colliding(collider, diff); break;
				default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
				}

				if (collision.Collided)
				{
					return Collision{ true, entity,
						sf::FloatRect{position, collider.GetBounds()},
						sf::FloatRect{ collision.Hitbox.left + otherTransform.Position.x, collision.Hitbox.top + otherTransform.Position.y,
									   collision.Hitbox.width, collision.Hitbox.height } };
				}
			}
			return Collision{ false, INVALID_ENTITY_ID, sf::FloatRect(), sf::FloatRect() };
		}

	private:
		Coordinator* m_Coordinator = nullptr;
	};

}
