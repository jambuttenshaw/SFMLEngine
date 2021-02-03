#pragma once

#include <SFML/Graphics.hpp>

#include "../ECS/Coordinator.h"
#include "../ECS/Components.h"

#include "../Systems/LightingSystem.h"

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
		
		void Init(Coordinator* coordinator, sf::RenderWindow* window, std::shared_ptr<LightingSystem> lightingSystem)
		{
			m_Coordinator = coordinator;
			m_RenderWindow = window;
			m_LightingSystem = lightingSystem;

			Material::GetAllMaterialsInUse(m_Materials);
		}

		void EntityAddedToSystem(Entity entity) override;

		void Render();

	private:
		void GetAllEntitiesWithMaterial(MaterialData material);
		void UploadLightingData(Material* material);

	private:
		Coordinator* m_Coordinator = nullptr;
		sf::RenderWindow* m_RenderWindow = nullptr;
		std::shared_ptr<LightingSystem> m_LightingSystem;

		std::vector<MaterialData> m_Materials;
		std::vector<Entity> m_CurrentEntities;

		int m_MaxOrderInLayer = 0;
		int m_MaxRenderLayer = 0;
		float m_OrderInLayerNormalizeFactor = 0.0f;
		float m_RenderLayerNormaizeFactor = 1.0f;

		sf::RenderStates m_RenderState;

		LightData m_Lights[MAX_LIGHTS];
		int m_NumLights = 0;
	};

}
