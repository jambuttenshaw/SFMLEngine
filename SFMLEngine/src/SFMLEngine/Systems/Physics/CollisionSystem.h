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

		Collision(Entity otherEntity, const sf::FloatRect& thisBounds, const sf::FloatRect& otherBounds)
			: Other(otherEntity), GlobalBounds(thisBounds), OtherGlobalBounds(otherBounds)
		{
			// work out the direction the collision occurred

			// the collision direction can be considered to be the direction from some point in this entity's collider
			// to the centre of the intersection rectangle

			// the difficulty is choosing which point to use inside this entity
			// the way I overcame this was by matching the major axis of the intersection to the same axis on this entity's collider
			// that way the direction from this entity to the intersection will always be correct

			// get the intersection rect and its center
			sf::FloatRect intersection = Math::Intersection(GlobalBounds, OtherGlobalBounds);
			sf::Vector2f collisionCentroid = Math::Centroid(intersection);

			// match the major axis  of this entity's bounds to the intersection and find its center
			sf::FloatRect matched = MatchMajorAxis(GlobalBounds, intersection);
			sf::Vector2f thisCentroid = Math::Centroid(matched);

			// the collision direction can now be considered to be the direction between these 2 points
			CollisionDirection = Math::GetDirection(collisionCentroid - thisCentroid);
		}

		sf::FloatRect MatchMajorAxis(const sf::FloatRect& toBeMatched, const sf::FloatRect& toMatch)
		{
			// create a copy
			sf::FloatRect newRect = toBeMatched;

			if (toMatch.width > toMatch.height)
			{
				// the x axis is the major axis of the rect
				newRect.left = toMatch.left;
				newRect.width = toMatch.width;
			}
			else
			{
				// the y axis is the major axis of the rect
				newRect.top = toMatch.top;
				newRect.height = toMatch.height;
			}

			return newRect;
		}
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
				
				// the data returned from the collision test include a bool saying if it was successful
				// and a floatrect which is the global bounds of the object collided with
				std::pair<bool, sf::FloatRect> collisionData;
				std::vector<sf::FloatRect> tilemapCollisions;
				switch (other.Type)
				{
				case ColliderType::Invalid:	SFMLE_CORE_ASSERT(0, "Invalid collider type!"); break;

				case ColliderType::Box:	
					collisionData = m_Coordinator->GetComponent<BoxCollider>(entity).Colliding(collider);
					if (collisionData.first)
						collisions.push_back(Collision{ entity, collider.GetGlobalBounds(), collisionData.second });

					break;

				case ColliderType::Circle:	
					collisionData = m_Coordinator->GetComponent<CircleCollider>(entity).Colliding(collider);
					if (collisionData.first)
						collisions.push_back(Collision{ entity, collider.GetGlobalBounds(), collisionData.second });

					break;

				case ColliderType::Tilemap:
					tilemapCollisions = m_Coordinator->GetComponent<TilemapCollider>(entity).Colliding(collider);
					for (auto const& c : tilemapCollisions)
						collisions.push_back(Collision{ entity, collider.GetGlobalBounds(), c });

					break;

				default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
				}
			}

			return collisions;
		}

	private:
		Coordinator* m_Coordinator = nullptr;
	};

}
