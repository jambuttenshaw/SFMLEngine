#pragma once

#include "CollisionSystem.h"
#include "SFMLEngine/LayerManager.h"


namespace SFMLEngine {

	class Physics
	{
	public:
		static void Init(Coordinator* coordinator, std::shared_ptr<CollisionSystem> collisionSystem);
		static void AddPhysicsLayer(Layer newLayer);

		static Layer GetPhysicsLayerMask(Layer layer);

		static void UnignoreCollisions(Layer layer1, Layer layer2);
		static void UnignoreCollisions(const std::string& layer1, const std::string& layer2)
		{
			UnignoreCollisions(LayerManager::LayerFromString(layer1), LayerManager::LayerFromString(layer2));
		}
		static void IgnoreCollisions(Layer layer1, Layer layer2);
		static void IgnoreCollisions(const std::string& layer1, const std::string& layer2)
		{
			IgnoreCollisions(LayerManager::LayerFromString(layer1), LayerManager::LayerFromString(layer2));
		}
		
		static std::pair<bool, Collision> CircleCast(const sf::Vector2f& centre, float radius, Layer layerMask = Layer{});

	public:
		static sf::Vector2f Gravity;

	private:
		static Coordinator* s_Coordinator;
		static std::shared_ptr<CollisionSystem> s_CollisionSystem;
		static std::unordered_map<Layer, Layer> s_LayerMasks;
	};

}
