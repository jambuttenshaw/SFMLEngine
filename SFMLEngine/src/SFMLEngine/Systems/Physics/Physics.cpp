#include "Physics.h"


#include "SFMLEngine/ECS/Components/Colliders/CircleCollider.h"
#include "SFMLEngine/ECS/Components/Transform.h"

#include "SFMLEngine/DebugTools.h"

namespace SFMLEngine {

	Coordinator* Physics::s_Coordinator = nullptr;
	std::shared_ptr<CollisionSystem> Physics::s_CollisionSystem = nullptr;
	std::unordered_map<Layer, Layer> Physics::s_LayerMasks;

	sf::Vector2f Physics::Gravity = { 0, 750 };

	void Physics::Init(Coordinator* coordinator, std::shared_ptr<CollisionSystem> collisionSystem)
	{
		s_Coordinator = coordinator;
		s_CollisionSystem = collisionSystem;

		// set up the default layer masks
		for (auto const& layer : LayerManager::GetAllLayers())
		{
			AddPhysicsLayer(layer);
		}
	}

	void Physics::AddPhysicsLayer(Layer newLayer)
	{
		Layer newMask = Layer{}.set();
		s_LayerMasks.insert(std::make_pair(newLayer, newMask));
	}

	Layer Physics::GetPhysicsLayerMask(Layer layer)
	{
		return s_LayerMasks[layer];
	}

	void Physics::UnignoreCollisions(Layer layer1, Layer layer2)
	{
		// set Layer2's bit in Layer1's mask, and vice-versa
		s_LayerMasks[layer1] |= layer2;
		s_LayerMasks[layer2] |= layer1;
	}

	void Physics::IgnoreCollisions(Layer layer1, Layer layer2)
	{
		// reset Layer2's but in Layer1's mask, and vice-versa
		s_LayerMasks[layer1] &= ~layer2;
		s_LayerMasks[layer2] &= ~layer1;
	}
	
	
	std::pair<bool, Collision> Physics::CircleCast(const sf::Vector2f& centre, float radius, Layer layerMask)
	{
		// since theres no draw circle debug option
		DEBUG_DRAW_RECT(sf::FloatRect{ centre.x - radius, centre.y - radius, 2 * radius, 2 * radius }, sf::Color::Red);

		for (auto const& collider : s_CollisionSystem->GetAllCollidersSharingPartition(sf::FloatRect{ centre.x - radius, centre.y - radius, 2 * radius, 2 * radius }))
		{
			// check to make sure this collider is compatible with the layer mask
			Layer colliderLayer = s_Coordinator->GetComponent<Identity>(collider->Owner).EntityLayer;
			// we only want to collide with colliders that agree with the layer mask
			// and that are not triggers
			if ((colliderLayer & layerMask) == colliderLayer && !collider->ColliderPtr->IsTrigger)
			{
				auto collisionTest = s_CollisionSystem->CircleCast(centre, radius, collider->ID);
				if (collisionTest.Other != INVALID_ENTITY_ID)
				{
					// collision occurred
					return std::make_pair(true, collisionTest);
				}
			}
		}

		return std::make_pair(false, Collision{});
	}


	std::pair<bool, Collision> Physics::BoxCast(const sf::FloatRect& rect, Layer layerMask)
	{
		// since theres no draw circle debug option
		DEBUG_DRAW_RECT(rect, sf::Color::Red);

		for (auto const& collider : s_CollisionSystem->GetAllCollidersSharingPartition(rect))
		{
			// check to make sure this collider is compatible with the layer mask
			Layer colliderLayer = s_Coordinator->GetComponent<Identity>(collider->Owner).EntityLayer;
			// we only want to collide with colliders that agree with the layer mask
			// and that are not triggers
			if ((colliderLayer & layerMask) == colliderLayer && !collider->ColliderPtr->IsTrigger)
			{
				auto collisionTest = s_CollisionSystem->BoxCast(rect, collider->ID);
				if (collisionTest.Other != INVALID_ENTITY_ID)
				{
					// collision occurred
					return std::make_pair(true, collisionTest);
				}
			}
		}

		return std::make_pair(false, Collision{});
	}
}
