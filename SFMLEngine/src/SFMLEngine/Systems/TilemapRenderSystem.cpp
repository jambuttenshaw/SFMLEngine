#include "TilemapRenderSystem.h"


namespace SFMLEngine {


	void TilemapRenderSystem::Init(Coordinator* coordinator, sf::RenderWindow* window)
	{
		m_Coordinator = coordinator;
		m_RenderWindow = window;
	}

	void TilemapRenderSystem::EntityAddedToSystem(Entity entity)
	{
		TilemapRenderer* tRenderer = &m_Coordinator->GetComponent<TilemapRenderer>(entity);
		Tilemap* tilemap = &m_Coordinator->GetComponent<Tilemap>(entity);
		Transform* transform = &m_Coordinator->GetComponent<Transform>(entity);

		m_MaxOrderInLayer = std::max(m_MaxOrderInLayer, abs(tRenderer->OrderInLayer));
		m_MaxRenderLayer = std::max(m_MaxRenderLayer, abs(tRenderer->RenderLayer));
		m_OrderInLayerNormalizeFactor = m_MaxOrderInLayer == 0 ? 0 : 1.0f / (float)(m_MaxOrderInLayer + 1.0f);
		m_RenderLayerNormaizeFactor = m_MaxRenderLayer == 0 ? 1.0f : 1.0f / (float)(m_MaxRenderLayer);

		m_TilemapRenderers.insert(std::make_pair(entity, tRenderer));
		m_Tilemaps.insert(std::make_pair(entity, tilemap));
		m_Transforms.insert(std::make_pair(entity, transform));
	}

	void TilemapRenderSystem::EntityRemovedFromSystem(Entity entity)
	{
		m_TilemapRenderers.erase(entity);
		m_Tilemaps.erase(entity);
		m_Transforms.erase(entity);
	}

	void TilemapRenderSystem::Update()
	{
		ZoneScoped;

		bool anyChanged = false;
		for (auto& e : m_Entities)
		{
			TilemapRenderer* tilemapRenderer = m_TilemapRenderers[e];
			if (ComponentModified(tilemapRenderer))
			{
				// if a sprite renderer has been modified then we need to update the order in layer factors etc
				m_MaxOrderInLayer = std::max(m_MaxOrderInLayer, abs(tilemapRenderer->OrderInLayer));
				m_MaxRenderLayer = std::max(m_MaxRenderLayer, abs(tilemapRenderer->RenderLayer));
				anyChanged = true;

				ResetModified(tilemapRenderer);
			}
		}

		if (anyChanged)
		{
			m_OrderInLayerNormalizeFactor = m_MaxOrderInLayer == 0 ? 0 : 1.0f / (float)(m_MaxOrderInLayer + 1.0f);
			m_RenderLayerNormaizeFactor = m_MaxRenderLayer == 0 ? 1.0f : 1.0f / (float)(m_MaxRenderLayer);
		}
	}

	void TilemapRenderSystem::Render()
	{
		ZoneScoped;

		if (!m_Entities.size()) return;

		for (const auto& entity : m_Entities)
		{
			ZoneScoped;
			ZoneName("DrawTilemap", 11);

			TilemapRenderer* tilemapRenderer = m_TilemapRenderers[entity];
			Tilemap* tilemap = m_Tilemaps[entity];
			Transform* transform = m_Transforms[entity];

			// get a pointer to the shader
			sf::Shader* shader = tilemapRenderer->MaterialPtr->GetShaderPtr();


			// set shader uniforms
			float depth = (tilemapRenderer->RenderLayer + (tilemapRenderer->OrderInLayer * m_OrderInLayerNormalizeFactor)) * m_RenderLayerNormaizeFactor;
			shader->setUniform("u_DepthValue", depth);
			
			// if its a lit material set the normal map and rotation to be applied to normals
			if (tilemapRenderer->MaterialPtr->IsLit())
			{
				// set the normal map to the normal map texture in the tile palette object
				shader->setUniform("u_NormalMap", *ResourceManager::GetResourceHandle<sf::Texture>(tilemap->PalettePtr->GetNormalMap()));

				// rotation value is used to compute transformed normals so lighting is correct for rotated sprites
				// requires negated because of the y axis being flipped
				shader->setUniform("u_Rotation", -transform->Rotation * DEG_TO_RAD);

			}

			// set up the render state
			// use the texture from the tile palette object
			m_RenderState.texture = ResourceManager::GetResourceHandle<sf::Texture>(tilemap->PalettePtr->GetTexture());
			m_RenderState.transform = transform->GetTransformMatrix();
			m_RenderState.shader = shader;

			{
				ZoneScoped;
				ZoneName("Draw", 4);
				m_RenderWindow->draw(tilemap->Geometry, m_RenderState);
			}
		}
	}

}

