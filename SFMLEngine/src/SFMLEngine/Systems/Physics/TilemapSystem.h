#pragma once


#include "../../ECS/Coordinator.h"


namespace SFMLEngine {

	struct Transform;
	struct Tilemap;
	struct TilemapCollider;

	class TilemapSystem : public System
	{
	public:
		TilemapSystem() = default;
		~TilemapSystem() = default;

		void Init(Coordinator* coordinator);

		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override;

		void Update();

	private:
		Coordinator* m_Coordinator = nullptr;

		std::unordered_map<Entity, Transform*> m_TransformCache;
		std::unordered_map<Entity, Tilemap*> m_TilemapCache;
		std::unordered_map<Entity, TilemapCollider*> m_ColliderCache;
	};

}

