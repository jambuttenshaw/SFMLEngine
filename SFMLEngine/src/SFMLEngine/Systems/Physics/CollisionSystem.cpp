#include "CollisionSystem.h"

#include "SFMLEngine/Systems/Physics/Physics.h"

#include "SFMLEngine/ECS/Components/Transform.h"
#include "SFMLEngine/ECS/Components/Colliders/BoxCollider.h"
#include "SFMLEngine/ECS/Components/Colliders/CircleCollider.h"
#include "SFMLEngine/ECS/Components/Colliders/TilemapCollider.h"


namespace SFMLEngine {

	std::queue<ColliderID> CollisionSystem::s_AvailableColliderIDs;
	size_t CollisionSystem::s_LivingColliderCount = 0;




	Collision::Collision(Entity otherEntity, const sf::FloatRect& thisBounds, const sf::FloatRect& otherBounds, ColliderID otherColliderID, bool trigger)
		: Other(otherEntity), GlobalBounds(thisBounds), OtherGlobalBounds(otherBounds), OtherColliderID(otherColliderID), Trigger(trigger)
	{
		// work out the direction the collision occurred

		// the collision direction can be considered to be the direction from some point in this entity's collider
		// to the centre of the intersection rectangle

		// the difficulty is choosing which point to use inside this entity
		// the way I overcame this was by matching the major axis of the intersection to the same axis on this entity's collider
		// that way the direction from this entity to the intersection will always be correct

		// get the intersection rect and its center
		sf::FloatRect intersection = Math::Intersection(GlobalBounds, OtherGlobalBounds);
		Centroid = Math::Centroid(intersection);

		// match the major axis  of this entity's bounds to the intersection and find its center
		sf::FloatRect matched = MatchMajorAxis(GlobalBounds, intersection);
		sf::Vector2f thisCentroid = Math::Centroid(matched);

		// the collision direction can now be considered to be the direction between these 2 points
		CollisionDirection = Math::GetDirection(Centroid - thisCentroid);
	}

	sf::FloatRect Collision::MatchMajorAxis(const sf::FloatRect& toBeMatched, const sf::FloatRect& toMatch)
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





	void CollisionSystem::Init(Coordinator* coordinator)
	{
		m_Coordinator = coordinator;
	}

	void CollisionSystem::EntityAddedToSystem(Entity entity)
	{
		ZoneScoped;

		Transform* transform = &m_Coordinator->GetComponent<Transform>(entity);
		auto const& collider = m_Coordinator->GetComponent<ColliderInfo>(entity);

		Collider* newCollider;
		switch (collider.Type)
		{
		case ColliderType::Invalid:	SFMLE_CORE_ASSERT(0, "Invalid collider type!"); break;
		case ColliderType::Box:
			SFMLE_CORE_ASSERT(m_Coordinator->HasComponent<BoxCollider>(entity), "Missing BoxCollider component, make sure collider is added before collider info.");
			newCollider = &m_Coordinator->GetComponent<BoxCollider>(entity);
			m_ColliderMap.insert(std::make_pair(newCollider->GetColliderID(), ColliderData{ entity, collider.Type, newCollider }));
			break;

		case ColliderType::Circle:	
			SFMLE_CORE_ASSERT(m_Coordinator->HasComponent<CircleCollider>(entity), "Missing CircleCollider component, make sure collider is added before collider info.");
			newCollider = &m_Coordinator->GetComponent<CircleCollider>(entity); 
			m_ColliderMap.insert(std::make_pair(newCollider->GetColliderID(), ColliderData{ entity, collider.Type, newCollider }));
			break;

		case ColliderType::Tilemap: 
			SFMLE_CORE_ASSERT(m_Coordinator->HasComponent<TilemapCollider>(entity), "Missing TilemapCollider component, make sure collider is added before collider info.");
			newCollider = &m_Coordinator->GetComponent<TilemapCollider>(entity); 
			for (auto& box : static_cast<TilemapCollider*>(newCollider)->GetCollisionGeometry())
			{
				m_ColliderMap.insert(std::make_pair(box.GetColliderID(), ColliderData{ entity, ColliderType::Box, &box }));
			}
			break;

		default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
		}

		newCollider->SetTransform(transform);
	}

	void CollisionSystem::EntityRemovedFromSystem(Entity entity)
	{
		// look through all of the colliders and see what ones belong to this entity
		std::vector<ColliderID> toErase;
		for (auto& collider : m_ColliderMap)
		{
			if (collider.second.Owner == entity)
			{
				toErase.push_back(collider.first);
			}
		}
		for (ColliderID collider : toErase)
			m_ColliderMap.erase(collider);
	}

	void CollisionSystem::DeleteTilemapColliderData(Entity tilemapCollider)
	{
		// look through all of the colliders and see what ones belong to this tilemap
		std::vector<ColliderID> toErase;
		for (auto& collider : m_ColliderMap)
		{
			if (collider.second.Owner == tilemapCollider)
			{
				toErase.push_back(collider.first);
			}
		}
		for (ColliderID collider : toErase)
			m_ColliderMap.erase(collider);
	}
	void CollisionSystem::AddTilemapColliderData(Entity tilemapCollider)
	{
		TilemapCollider* collider = &m_Coordinator->GetComponent<TilemapCollider>(tilemapCollider);
		for (auto& box : static_cast<TilemapCollider*>(collider)->GetCollisionGeometry())
		{
			m_ColliderMap.insert(std::make_pair(box.GetColliderID(), ColliderData{ tilemapCollider, ColliderType::Box, &box }));
		}
	}


	const Collision CollisionSystem::TestCollision(Entity entity, ColliderID other)
	{
		auto const& colliderData = m_ColliderMap[other];

		// make sure entity and other are in layers that can collide
		Layer entityLayer = m_Coordinator->GetComponent<Identity>(entity).EntityLayer;
		Layer otherLayer = m_Coordinator->GetComponent<Identity>(colliderData.Owner).EntityLayer;

		if ((entityLayer & Physics::GetPhysicsLayerMask(otherLayer)) == entityLayer || (colliderData.ColliderPtr->IsTrigger))
		{
			auto& colliderInfo = m_Coordinator->GetComponent<ColliderInfo>(entity);
			switch (colliderInfo.Type)
			{
			case ColliderType::Invalid:	SFMLE_CORE_ASSERT(0, "Invalid collider type!"); break;
			case ColliderType::Box:
				return RunCollisionTest(m_Coordinator->GetComponent<BoxCollider>(entity), other);
				break;

			case ColliderType::Circle:
				return RunCollisionTest(m_Coordinator->GetComponent<CircleCollider>(entity), other);
				break;

			case ColliderType::Tilemap: SFMLE_CORE_ASSERT(0, "Simulating physics on tilemaps is currently not supported."); break;
			default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
			}
		}

		// no collision was tested
		return Collision{ INVALID_ENTITY_ID, sf::FloatRect(), sf::FloatRect(), NULL_COLLIDER_ID };
	}




	void CollisionSystem::SetupColliderIDs()
	{
		ZoneScoped;
		for (ColliderID colliderID = 0; colliderID < MAX_COLLIDERS; ++colliderID)
		{
			s_AvailableColliderIDs.push(colliderID);
		}
	}

	ColliderID CollisionSystem::GetNextColliderID()
	{
		SFMLE_CORE_ASSERT(s_LivingColliderCount < MAX_COLLIDERS, "Too many colliders in existance!");

		ColliderID nextID = s_AvailableColliderIDs.front();
		s_AvailableColliderIDs.pop();
		++s_LivingColliderCount;

		return nextID;
	}

	void CollisionSystem::ReplaceColliderID(ColliderID id)
	{
		if (id == NULL_COLLIDER_ID) return;

		s_AvailableColliderIDs.push(id);
		--s_LivingColliderCount;
	}


}
