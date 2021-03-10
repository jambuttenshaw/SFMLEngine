#pragma once

#include "CollisionSystem.h"


namespace SFMLEngine {

	class Physics
	{
	public:
		static void Init(std::shared_ptr<CollisionSystem> collisionSystem);

		static Layer GetLayerMask(Layer layer);
		

		static std::vector<Collision> CollisionAtPoint(const sf::Vector2f& point, float radius, Layer layerMask = Layer{});

	public:
		static sf::Vector2f Gravity;

	private:
		static std::shared_ptr<CollisionSystem> s_CollisionSystem;
		static std::unordered_map<Layer, Layer> s_LayerMasks;
	};

}
