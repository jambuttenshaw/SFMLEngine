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

		void Render(ResourceID material, int renderLayer);

		const std::set<int>& GetRenderLayersUsed() { return m_RenderLayers; }

	private:
		Coordinator* m_Coordinator = nullptr;
		sf::RenderWindow* m_RenderWindow = nullptr;

		std::unordered_map<ResourceID, std::set<Entity>> m_MaterialsMap;
		std::unordered_map<Entity, TilemapRenderer*> m_TilemapRenderers;
		std::unordered_map<Entity, Tilemap*> m_Tilemaps;
		std::unordered_map<Entity, Transform*> m_Transforms;

		std::set<int> m_RenderLayers;

		sf::RenderStates m_RenderState;
	};

}

