#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/Math.h"
#include "SFMLEngine/ECS/Coordinator.h"

#include "SFMLEngine/ECS/Components/Identity.h"

#include "SFMLEngine/ECS/Components/Colliders/Collider.h"

#include "SFMLEngine/ECS/Components/Colliders/BoxCollider.h"
#include "SFMLEngine/ECS/Components/Colliders/CircleCollider.h"
#include "SFMLEngine/ECS/Components/Colliders/TilemapCollider.h"


namespace SFMLEngine {

	struct Collision
	{
		Math::Direction CollisionDirection;
		Entity Other;
		sf::FloatRect GlobalBounds; // the boundaries of this objects collider
		sf::FloatRect OtherGlobalBounds; // the boundaries of the object the collision occurred with

		ColliderID OtherColliderID;

		Collision(Entity otherEntity, const sf::FloatRect& thisBounds, const sf::FloatRect& otherBounds, ColliderID otherColliderID);
		
	private:
		sf::FloatRect MatchMajorAxis(const sf::FloatRect& toBeMatched, const sf::FloatRect& toMatch);
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

		const std::set<Entity>& GetCollideableEntities() { return m_Entities; }

		const std::vector<Collision> TestCollision(Entity entity);

	private:
		template<typename T>
		const std::vector<Collision> DoCollisionTest(T& collider, Layer layerMask, Entity thisEntity = INVALID_ENTITY_ID)
		{
			ZoneScoped;

			std::vector<Collision> collisions;

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
				
				std::vector<std::pair<ColliderID, sf::FloatRect>> collisionData;
				switch (other.Type)
				{
				case ColliderType::Invalid:	SFMLE_CORE_ASSERT(0, "Invalid collider type!"); break;

				case ColliderType::Box:	
					collisionData.push_back(m_Coordinator->GetComponent<BoxCollider>(entity).Colliding(collider));

					break;

				case ColliderType::Circle:	
					collisionData.push_back(m_Coordinator->GetComponent<CircleCollider>(entity).Colliding(collider));

					break;

				case ColliderType::Tilemap:
					collisionData = m_Coordinator->GetComponent<TilemapCollider>(entity).Colliding(collider);

					break;

				default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
				}

				for (auto const& c : collisionData)
				{
					if (c.first != NULL_COLLIDER_ID)
					{
						collisions.push_back(Collision{ entity, collider.GetGlobalBounds(), c.second, c.first });
					}
				}
			}

			return collisions;
		}


	public:
		static void SetupColliderIDs();
		static ColliderID GetNextColliderID();
		static void ReplaceColliderID(ColliderID id);


	private:
		Coordinator* m_Coordinator = nullptr;


	private:
		static std::queue<ColliderID> s_AvailableColliderIDs;
		static size_t s_LivingColliderCount;

	};

}
