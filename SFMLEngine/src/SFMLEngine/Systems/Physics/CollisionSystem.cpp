#include "CollisionSystem.h"

#include "SFMLEngine/Systems/Physics/Physics.h"

#include "SFMLEngine/ECS/Components/Transform.h"
#include "SFMLEngine/ECS/Components/Colliders/BoxCollider.h"
#include "SFMLEngine/ECS/Components/Colliders/CircleCollider.h"
#include "SFMLEngine/ECS/Components/Colliders/TilemapCollider.h"


namespace SFMLEngine {

	std::queue<ColliderID> CollisionSystem::s_AvailableColliderIDs;
	size_t CollisionSystem::s_LivingColliderCount = 0;




	Collision::Collision(Entity otherEntity, const sf::FloatRect& thisBounds, const sf::FloatRect& otherBounds, ColliderID otherColliderID)
		: Other(otherEntity), GlobalBounds(thisBounds), OtherGlobalBounds(otherBounds), OtherColliderID(otherColliderID)
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

		switch (collider.Type)
		{
		case ColliderType::Invalid:	SFMLE_CORE_ASSERT(0, "Invalid collider type!"); break;
		case ColliderType::Box:		m_Coordinator->GetComponent<BoxCollider>(entity).SetTransform(transform); break;
		case ColliderType::Circle:	m_Coordinator->GetComponent<CircleCollider>(entity).SetTransform(transform); break;
		case ColliderType::Tilemap: m_Coordinator->GetComponent<TilemapCollider>(entity).SetTransform(transform); break;
		default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
		}
	}

	void CollisionSystem::EntityRemovedFromSystem(Entity entity)
	{

	}


	const std::vector<Collision> CollisionSystem::TestCollision(Entity entity)
	{
		ZoneScoped;

		auto const& collider = m_Coordinator->GetComponent<ColliderInfo>(entity);
		auto mask = Physics::GetLayerMask(m_Coordinator->GetComponent<Identity>(entity).EntityLayer);

		switch (collider.Type)
		{
		case ColliderType::Invalid:	SFMLE_CORE_ASSERT(0, "Invalid collider type!"); break;
		case ColliderType::Box:		return DoCollisionTest(m_Coordinator->GetComponent<BoxCollider>(entity), mask, entity); break;
		case ColliderType::Circle:	return DoCollisionTest(m_Coordinator->GetComponent<CircleCollider>(entity), mask, entity); break;
		case ColliderType::Tilemap: return DoCollisionTest(m_Coordinator->GetComponent<TilemapCollider>(entity), mask, entity); break;
		default:					SFMLE_CORE_ASSERT(0, "Unknown collider type!"); break;
		}

		// this shouldn't be reached
		return std::vector<Collision>();
	}






	void CollisionSystem::SetupColliderIDs()
	{
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
