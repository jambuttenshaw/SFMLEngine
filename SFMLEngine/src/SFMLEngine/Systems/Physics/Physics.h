#pragma once

#include "CollisionSystem.h"
#include "SFMLEngine/LayerManager.h"


namespace SFMLEngine {

	class Physics
	{
	public:
		static void Init(std::shared_ptr<CollisionSystem> collisionSystem);
		static void AddPhysicsLayer(Layer newLayer);

		static Layer GetPhysicsLayerMask(Layer layer);

		static void AllowCollisions(Layer layer1, Layer layer2);
		static void AllowCollisions(const std::string& layer1, const std::string& layer2)
		{
			AllowCollisions(LayerManager::LayerFromString(layer1), LayerManager::LayerFromString(layer2));
		}
		static void DisallowCollisions(Layer layer1, Layer layer2);
		static void DisallowCollisions(const std::string& layer1, const std::string& layer2)
		{
			DisallowCollisions(LayerManager::LayerFromString(layer1), LayerManager::LayerFromString(layer2));
		}
		
		// static std::vector<Collision> CollisionAtPoint(const sf::Vector2f& point, float radius, Layer layerMask = Layer{});

	public:
		static sf::Vector2f Gravity;

	private:
		static std::shared_ptr<CollisionSystem> s_CollisionSystem;
		static std::unordered_map<Layer, Layer> s_LayerMasks;
	};

}
