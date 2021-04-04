#include "CollisionSystem.h"

#include "SFMLEngine/Systems/Physics/Physics.h"

#include "SFMLEngine/ECS/Components/Transform.h"
#include "SFMLEngine/ECS/Components/Colliders/BoxCollider.h"
#include "SFMLEngine/ECS/Components/Colliders/CircleCollider.h"
#include "SFMLEngine/ECS/Components/Colliders/TilemapCollider.h"


#include "SpatialPartition.h"


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


	CollisionSystem::CollisionSystem()
	{
		m_SpatialPartition = new SpatialPartition(MAX_TILEMAP_COLLIDER_SIZE * 32); 
	}
	CollisionSystem::~CollisionSystem()
	{ 
		delete m_SpatialPartition; 
	}
	const std::vector<ColliderData*> CollisionSystem::GetAllCollidersSharingPartition(const sf::FloatRect& rect)
	{ 
		return m_SpatialPartition->CollidersSharingPartition(rect);
	}
	
	const std::vector<ColliderData*> CollisionSystem::GetAllCollidersSharingPartition(Entity entity)
	{
		sf::FloatRect rect;

		auto& colliderInfo = m_Coordinator->GetComponent<ColliderInfo>(entity);
		switch (colliderInfo.Type)
		{
		case ColliderType::Invalid:	SFMLE_CORE_ASSERT(0, "Invalid collider type!"); break;
		case ColliderType::Box:		rect = m_Coordinator->GetComponent<BoxCollider>(entity).GetGlobalBounds(); break;
		case ColliderType::Circle:	rect = m_Coordinator->GetComponent<CircleCollider>(entity).GetGlobalBounds(); break;
		case ColliderType::Tilemap: SFMLE_CORE_ASSERT(0, "Simulating physics on tilemaps is currently not supported."); break;
		default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
		}
		return GetAllCollidersSharingPartition(rect);
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
			newCollider->SetTransform(transform);

			m_SpatialPartition->Insert(ColliderData{ newCollider->GetColliderID(), entity, ColliderType::Box, newCollider });
			break;

		case ColliderType::Circle:	
			SFMLE_CORE_ASSERT(m_Coordinator->HasComponent<CircleCollider>(entity), "Missing CircleCollider component, make sure collider is added before collider info.");
			newCollider = &m_Coordinator->GetComponent<CircleCollider>(entity); 
			newCollider->SetTransform(transform);

			m_SpatialPartition->Insert(ColliderData{ newCollider->GetColliderID(), entity, ColliderType::Box, newCollider });
			break;

		case ColliderType::Tilemap: 
			SFMLE_CORE_ASSERT(m_Coordinator->HasComponent<TilemapCollider>(entity), "Missing TilemapCollider component, make sure collider is added before collider info.");
			newCollider = &m_Coordinator->GetComponent<TilemapCollider>(entity); 
			newCollider->SetTransform(transform);

			for (auto& box : static_cast<TilemapCollider*>(newCollider)->GetCollisionGeometry())
			{
				m_SpatialPartition->Insert(ColliderData{ box.GetColliderID(), entity, ColliderType::Box, &box });
			}
			break;

		default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
		}

	}

	void CollisionSystem::EntityRemovedFromSystem(Entity entity)
	{
		m_SpatialPartition->Erase(entity);
	}

	void CollisionSystem::DeleteTilemapColliderData(Entity tilemapCollider)
	{
		m_SpatialPartition->Erase(tilemapCollider);
	}


	void CollisionSystem::AddTilemapColliderData(Entity tilemapCollider)
	{
		TilemapCollider* collider = &m_Coordinator->GetComponent<TilemapCollider>(tilemapCollider);
		for (auto& box : static_cast<TilemapCollider*>(collider)->GetCollisionGeometry())
		{
			// m_ColliderMap.insert(std::make_pair(box.GetColliderID(), ColliderData{ box.GetColliderID(), tilemapCollider, ColliderType::Box, &box }));
			m_SpatialPartition->Insert(ColliderData{ box.GetColliderID(), tilemapCollider, ColliderType::Box, &box });
		}
	}


	const Collision CollisionSystem::TestCollision(Entity entity, const ColliderData* const other)
	{
		// make sure entity and other are in layers that can collide
		Layer entityLayer = m_Coordinator->GetComponent<Identity>(entity).EntityLayer;
		Layer otherLayer = m_Coordinator->GetComponent<Identity>(other->Owner).EntityLayer;

		if ((entityLayer & Physics::GetPhysicsLayerMask(otherLayer)) == entityLayer || (other->ColliderPtr->IsTrigger))
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

	const Collision CollisionSystem::CircleCast(const sf::Vector2f& centre, float radius, const ColliderData* const other)
	{

		std::pair<bool, sf::FloatRect> collisionData;
		switch (other->Type)
		{
		case ColliderType::Invalid:	SFMLE_CORE_ASSERT(0, "Invalid collider type!"); break;
		case ColliderType::Box:
			collisionData = static_cast<BoxCollider*>(other->ColliderPtr)->Colliding(centre, radius);
			break;

		case ColliderType::Circle:
			collisionData = static_cast<CircleCollider*>(other->ColliderPtr)->Colliding(centre, radius);
			break;

		case ColliderType::Tilemap: SFMLE_CORE_ASSERT(0, "Internal collider mapping error occurred."); break;
		default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
		}

		if (collisionData.first)
		{
			return Collision
			{
				other->Owner,
				sf::FloatRect{centre.x - radius, centre.y - radius, 2 * radius, 2 * radius},
				collisionData.second,
				other->ID,
				other->ColliderPtr->IsTrigger
			};
		}
		else
		{
			return Collision{ INVALID_ENTITY_ID, sf::FloatRect(), sf::FloatRect(), NULL_COLLIDER_ID };
		}
	}

	const Collision CollisionSystem::BoxCast(const sf::FloatRect& rect, const ColliderData* const other)
	{
		std::pair<bool, sf::FloatRect> collisionData;
		switch (other->Type)
		{
		case ColliderType::Invalid:	SFMLE_CORE_ASSERT(0, "Invalid collider type!"); break;
		case ColliderType::Box:
			collisionData = static_cast<BoxCollider*>(other->ColliderPtr)->Colliding(rect);
			break;

		case ColliderType::Circle:
			collisionData = static_cast<CircleCollider*>(other->ColliderPtr)->Colliding(rect);
			break;

		case ColliderType::Tilemap: SFMLE_CORE_ASSERT(0, "Internal collider mapping error occurred."); break;
		default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
		}

		if (collisionData.first)
		{
			return Collision
			{
				other->Owner,
				rect,
				collisionData.second,
				other->ID,
				other->ColliderPtr->IsTrigger
			};
		}
		else
		{
			return Collision{ INVALID_ENTITY_ID, sf::FloatRect(), sf::FloatRect(), NULL_COLLIDER_ID };
		}
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

	void CollisionSystem::DeletedColliderID(ColliderID id)
	{
		if (id == NULL_COLLIDER_ID) return;

		s_AvailableColliderIDs.push(id);
		--s_LivingColliderCount;
	}


}
