#include "Physics.h"


#include "SFMLEngine/ECS/Components/Colliders/CircleCollider.h"
#include "SFMLEngine/ECS/Components/Transform.h"

#include "SFMLEngine/LayerManager.h"

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
			Layer newMask = Layer{}.set(1);
			s_LayerMasks.insert(std::make_pair(layer, newMask));
		}
	}

	Layer Physics::GetLayerMask(Layer layer)
	{
		return s_LayerMasks[layer];
	}
	
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

}
