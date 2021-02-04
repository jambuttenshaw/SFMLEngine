#pragma once

#include <SFML/Graphics.hpp>

#include "../ECS/Coordinator.h"
#include "../ECS/Components.h"

#include "Lighting/PointLightSystem.h"

#include "../ResourceManagement/ResourceManager.h"

#include "../Renderer/Renderer.h"
#include "../Renderer/ShaderLibrary.h"
#include "../Renderer/Material.h"


namespace SFMLEngine {

	class RenderSystem : public System
	{
	public:
		RenderSystem() = default;
		~RenderSystem() = default;
		
		void Init(Coordinator* coordinator, sf::RenderWindow* window);

		void EntityAddedToSystem(Entity entity) override;

		void Render();

	private:
		void GetAllEntitiesWithMaterial(MaterialData material);
		void UploadLightingData(Material* material);

	private:
		Coordinator* m_Coordinator = nullptr;
		sf::RenderWindow* m_RenderWindow = nullptr;
		std::shared_ptr<PointLightSystem> m_PointLightSystem;

		std::vector<MaterialData> m_Materials;
		std::vector<Entity> m_CurrentEntities;

		int m_MaxOrderInLayer = 0;
		int m_MaxRenderLayer = 0;
		float m_OrderInLayerNormalizeFactor = 0.0f;
		float m_RenderLayerNormaizeFactor = 1.0f;

		sf::RenderStates m_RenderState;

		PointLightData m_Lights[MAX_LIGHTS];
		int m_NumLights = 0;
	};

}
