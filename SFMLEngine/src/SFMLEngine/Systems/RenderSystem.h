#pragma once

#include <SFML/Graphics.hpp>

#include "../ECS/Coordinator.h"

#include "../Renderer/Material.h"


namespace SFMLEngine {

	class RenderSystem : public System
	{
	public:
		RenderSystem() = default;
		~RenderSystem() = default;
		
		void Init(Coordinator* coordinator, sf::RenderWindow* window);

		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override {}

		void Update();
		void Render();

	private:
		void GetAllEntitiesWithMaterial(MaterialData material);

	private:
		Coordinator* m_Coordinator = nullptr;
		sf::RenderWindow* m_RenderWindow = nullptr;

		std::vector<Entity> m_CurrentEntities;

		int m_MaxOrderInLayer = 0;
		int m_MaxRenderLayer = 0;
		float m_OrderInLayerNormalizeFactor = 0.0f;
		float m_RenderLayerNormaizeFactor = 1.0f;

		sf::RenderStates m_RenderState;
	};

}
