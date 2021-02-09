#pragma once

#include <SFML/Graphics.hpp>

#include "../ECS/Coordinator.h"
#include "../ECS/Components/Transform.h"
#include "../ECS/Components/SpriteRenderer.h"

#include "../Renderer/Material.h"

namespace SFMLEngine {

	class SpriteRenderSystem : public System
	{
	public:
		SpriteRenderSystem() = default;
		~SpriteRenderSystem() = default;
		
		void Init(Coordinator* coordinator, sf::RenderWindow* window);

		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override;

		void Update();
		void Render();

	private:
		Coordinator* m_Coordinator = nullptr;
		sf::RenderWindow* m_RenderWindow = nullptr;

		std::unordered_map<Entity, SpriteRenderer*> m_SpriteRenderers;
		std::unordered_map<Entity, Transform*> m_Transforms;

		int m_MaxOrderInLayer = 0;
		int m_MaxRenderLayer = 0;
		float m_OrderInLayerNormalizeFactor = 0.0f;
		float m_RenderLayerNormaizeFactor = 1.0f;

		sf::RenderStates m_RenderState;
	};

}
