#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/Coordinator.h"

#include "SFMLEngine/ECS/Components/Identity.h"

#include "SFMLEngine/ECS/Components/Colliders/Collider.h"

#include "SFMLEngine/ECS/Components/Colliders/BoxCollider.h"
#include "SFMLEngine/ECS/Components/Colliders/CircleCollider.h"
#include "SFMLEngine/ECS/Components/Colliders/TilemapCollider.h"


namespace SFMLEngine {

	struct Collision
	{
		bool Collided;
		Entity Other;
		sf::FloatRect GlobalBounds; // the boundaries of this objects collider
		sf::FloatRect OtherGlobalBounds; // the boundaries of the object the collision occurred with
	};

	class CollisionSystem : public System
	{
	public:
		friend class Physics;

		CollisionSystem() = default;
		~CollisionSystem() = default;

		void Init(Coordinator* coordinator);


		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override;

		const Collision TestCollision(Entity entity);

	private:
		template<typename T>
		const Collision DoCollisionTest(T& collider, Layer layerMask, Entity thisEntity = INVALID_ENTITY_ID)
		{
			ZoneScoped;
			for (auto& entity : m_Entities)
			{
				ZoneScoped;
				ZoneName("ProcessEntity", 13);

				// dont test collision against its own collider
				if (entity == thisEntity) continue;

				// make sure this entity applies to the layer mask
				auto& id = m_Coordinator->GetComponent<Identity>(entity);
				// if the set bit in the entity layer is not in the mask then skip this entity
				if ((id.EntityLayer & layerMask) != id.EntityLayer) continue;

				// find out what type of collider this object has
				ColliderInfo& other = m_Coordinator->GetComponent<ColliderInfo>(entity);
				
				// the data returned from the collision test include a bool saying if it was successful
				// and a floatrect which is the global bounds of the object collided with
				std::pair<bool, sf::FloatRect> collisionData;
				switch (other.Type)
				{
				case ColliderType::Invalid:	SFMLE_CORE_ASSERT(0, "Invalid collider type!"); break;
				case ColliderType::Box:		collisionData =  m_Coordinator->GetComponent<BoxCollider>(entity).Colliding(collider);
					break;
				case ColliderType::Circle:	collisionData = m_Coordinator->GetComponent<CircleCollider>(entity).Colliding(collider);
					break;									   
				case ColliderType::Tilemap: collisionData = m_Coordinator->GetComponent<TilemapCollider>(entity).Colliding(collider);
					break;
				default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
				}

				if (collisionData.first)
				{
					return Collision{ true, entity,
						collider.GetGlobalBounds(),
						collisionData.second
					};
				}
			}
			return Collision{ false, INVALID_ENTITY_ID, sf::FloatRect(), sf::FloatRect() };
		}

	private:
		Coordinator* m_Coordinator = nullptr;
	};

}
