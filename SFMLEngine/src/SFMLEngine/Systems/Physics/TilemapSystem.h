#pragma once


#include "SFMLEngine/ECS/Coordinator.h"


namespace SFMLEngine {

	class Transform;
	class Tilemap;
	class TilemapCollider;

	class CollisionSystem;

	class TilemapSystem : public System
	{
	public:
		TilemapSystem() = default;
		~TilemapSystem() = default;

		void Init(Coordinator* coordinator, std::shared_ptr<CollisionSystem> collisionSystem);

		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override;

		void Update();

	private:
		Coordinator* m_Coordinator = nullptr;
		std::shared_ptr<CollisionSystem> m_CollisionSystem;

		std::unordered_map<Entity, Transform*> m_TransformCache;
		std::unordered_map<Entity, Tilemap*> m_TilemapCache;
		std::unordered_map<Entity, TilemapCollider*> m_ColliderCache;
	};

}

