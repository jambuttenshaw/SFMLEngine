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

	class SpatialPartition;


	struct Collision
	{
		Math::Direction CollisionDirection;
		sf::Vector2f Centroid;

		Entity Other;
		sf::FloatRect GlobalBounds; // the boundaries of this objects collider
		sf::FloatRect OtherGlobalBounds; // the boundaries of the object the collision occurred with

		ColliderID OtherColliderID;

		bool Trigger;

		Collision() {};
		Collision(Entity otherEntity, const sf::FloatRect& thisBounds, const sf::FloatRect& otherBounds, ColliderID otherColliderID, bool trigger = false);
		
	private:
		sf::FloatRect MatchMajorAxis(const sf::FloatRect& toBeMatched, const sf::FloatRect& toMatch);
	};

	struct ColliderData
	{
		ColliderID ID;
		Entity Owner;
		ColliderType Type;
		Collider* ColliderPtr;
	};

	class CollisionSystem : public System
	{
	public:
		friend class Physics;

		CollisionSystem();
		~CollisionSystem();

		void Init(Coordinator* coordinator);


		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override;

		const std::set<Entity>& GetCollideableEntities() { return m_Entities; }
		const std::unordered_map<ColliderID, ColliderData>& GetAllColliders() { return m_ColliderMap; }


		const std::vector<ColliderData*> GetAllCollidersSharingPartition(const sf::FloatRect& rect);
		const std::vector<ColliderData*> GetAllCollidersSharingPartition(Entity entity);


		// for tilemaps to use
		// when a tilemap collider is regenerated, it will invalidate all of the pointers
		// and ColliderID's associated with it
		// so old ones need to be removed and new ones put in
		void DeleteTilemapColliderData(Entity tilemapCollider);
		void AddTilemapColliderData(Entity tilemapCollider);


		const Collision TestCollision(Entity entity, const ColliderData& other);

		// testing against circle
		const Collision CircleCast(const sf::Vector2f& centre, float radius, ColliderID other);
		// testing against box
		const Collision BoxCast(const sf::FloatRect& rect, ColliderID other);

	private:
		template <typename T>
		const Collision RunCollisionTest(T& collider, const ColliderData& other)
		{
			std::pair<bool, sf::FloatRect> collisionData;

			switch (other.Type)
			{
			case ColliderType::Invalid:	SFMLE_CORE_ASSERT(0, "Invalid collider type!"); break;
			case ColliderType::Box:
				collisionData = static_cast<BoxCollider*>(other.ColliderPtr)->Colliding(collider);
				break;

			case ColliderType::Circle:
				collisionData = static_cast<CircleCollider*>(other.ColliderPtr)->Colliding(collider);
				break;

			case ColliderType::Tilemap: SFMLE_CORE_ASSERT(0, "Tilemap collider is an invalid collision primitive."); break;
			default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
			}

			if (collisionData.first)
			{
				return Collision
				{
					other.Owner,
					collider.GetGlobalBounds(),
					collisionData.second,
					other.ID,
					other.ColliderPtr->IsTrigger
				};
			}
			else
			{
				return Collision{ INVALID_ENTITY_ID, sf::FloatRect(), sf::FloatRect(), NULL_COLLIDER_ID };
			}
		}


	public:
		static void SetupColliderIDs();
		static ColliderID GetNextColliderID();
		static void ReplaceColliderID(ColliderID id);


	private:
		Coordinator* m_Coordinator = nullptr;
		std::unordered_map<ColliderID, ColliderData> m_ColliderMap;

		SpatialPartition* m_SpatialPartition;

	private:
		static std::queue<ColliderID> s_AvailableColliderIDs;
		static size_t s_LivingColliderCount;

	};

}
