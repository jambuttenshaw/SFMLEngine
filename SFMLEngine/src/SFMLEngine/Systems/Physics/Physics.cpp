#include "Physics.h"


#include "SFMLEngine/ECS/Components/Colliders/CircleCollider.h"
#include "SFMLEngine/ECS/Components/Transform.h"

#include "SFMLEngine/DebugTools.h"

namespace SFMLEngine {

	std::shared_ptr<CollisionSystem> Physics::s_CollisionSystem = nullptr;
	std::unordered_map<Layer, Layer> Physics::s_LayerMasks;

	sf::Vector2f Physics::Gravity = { 0, 750 };

	void Physics::Init(std::shared_ptr<CollisionSystem> collisionSystem)
	{
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

	void Physics::AllowCollisions(Layer layer1, Layer layer2)
	{
		// set Layer2's bit in Layer1's mask, and vice-versa
		s_LayerMasks[layer1] |= layer2;
		s_LayerMasks[layer2] |= layer1;
	}

	void Physics::DisallowCollisions(Layer layer1, Layer layer2)
	{
		// reset Layer2's but in Layer1's mask, and vice-versa
		s_LayerMasks[layer1] &= ~layer2;
		s_LayerMasks[layer2] &= ~layer1;
	}
	
	/*
	std::vector<Collision> Physics::CollisionAtPoint(const sf::Vector2f& point, float radius, Layer layerMask)
	{
		// run a collision test at the point
		// creates a circle collider at that point in space
		// and see if it collides with any other colliders

		CircleCollider collider{ radius, point - sf::Vector2f{ radius, radius} };
		Transform t{};
		collider.SetTransform(&t);

		DEBUG_DRAW_RECT(point - sf::Vector2f{ radius, radius }, 2.0f * sf::Vector2f{ radius, radius }, sf::Color::Red);

		return s_CollisionSystem->DoCollisionTest(collider, layerMask);
	}
	*/
}
