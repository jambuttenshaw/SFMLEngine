#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/Coordinator.h"
#include "SFMLEngine/ECS/Components/Transform.h"
#include "SFMLEngine/ECS/Components/Tilemap.h"
#include "SFMLEngine/ECS/Components/TilemapRenderer.h"
#include "SFMLEngine/Renderer/Material.h"



namespace SFMLEngine {

	class TilemapRenderSystem : public System
	{
	public:

		TilemapRenderSystem() = default;
		~TilemapRenderSystem() = default;

		void Init(Coordinator* coordinator, sf::RenderWindow* window);

		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override;

		void Update();
		void Render();


	private:
		Coordinator* m_Coordinator = nullptr;
		sf::RenderWindow* m_RenderWindow = nullptr;

		std::unordered_map<Entity, TilemapRenderer*> m_TilemapRenderers;
		std::unordered_map<Entity, Tilemap*> m_Tilemaps;
		std::unordered_map<Entity, Transform*> m_Transforms;

		int m_MaxOrderInLayer = 0;
		int m_MaxRenderLayer = 0;
		float m_OrderInLayerNormalizeFactor = 0.0f;
		float m_RenderLayerNormaizeFactor = 1.0f;

		sf::RenderStates m_RenderState;
	};

}

