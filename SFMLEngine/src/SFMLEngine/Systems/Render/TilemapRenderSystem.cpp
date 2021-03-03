#include "TilemapRenderSystem.h"

#include "SFMLEngine/Math.h"
#include "SFMLEngine/Renderer/Renderer.h"

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
		m_OrderInLayerNormalizeFactor = Renderer::CalculateOrderInLayerFactor(m_MaxOrderInLayer);
		m_RenderLayerNormaizeFactor = Renderer::CalculateRenderLayerFactor(m_MaxRenderLayer);
		
		m_TilemapRenderers.insert(std::make_pair(entity, tRenderer));
		m_Tilemaps.insert(std::make_pair(entity, tilemap));
		m_Transforms.insert(std::make_pair(entity, transform));

		// insert into materials map
		if (m_MaterialsMap.find(tRenderer->MaterialHandle) == m_MaterialsMap.end())
		{
			m_MaterialsMap.insert(std::make_pair(tRenderer->MaterialHandle, std::set<Entity>{ entity }));
		}
		else
		{
			m_MaterialsMap[tRenderer->MaterialHandle].insert(entity);
		}
	}

	void TilemapRenderSystem::EntityRemovedFromSystem(Entity entity)
	{
		auto mat = m_TilemapRenderers[entity]->MaterialHandle;
		m_MaterialsMap[mat].erase(entity);
		if (m_MaterialsMap[mat].empty())
			m_MaterialsMap.erase(mat);

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
			m_OrderInLayerNormalizeFactor = Renderer::CalculateOrderInLayerFactor(m_MaxOrderInLayer);
			m_RenderLayerNormaizeFactor = Renderer::CalculateRenderLayerFactor(m_MaxRenderLayer);
		}
	}

	void TilemapRenderSystem::Render(ResourceID material)
	{
		ZoneScoped;

		if (m_MaterialsMap[material].empty()) return;

		for (const auto& entity : m_MaterialsMap[material])
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
			shader->setUniform("u_DepthValue", -depth);
			
			// if its a lit material set the normal map and rotation to be applied to normals
			if (tilemapRenderer->MaterialPtr->IsLit())
			{
				ZoneScoped;
				ZoneName("SetLighting", 11);

				// set the normal map to the normal map texture in the tile palette object
				shader->setUniform("u_NormalMap", *ResourceManager::GetResourceHandle<sf::Texture>(tilemap->PalettePtr->GetNormalMap()));

				shader->setUniform("u_PosOffset", transform->Position);

				// rotation value is used to compute transformed normals so lighting is correct for rotated sprites
				// requires negated because of the y axis being flipped
				shader->setUniform("u_Rotation", -transform->Rotation * Math::DEG_TO_RAD);

			}

			// set up the render state
			// use the texture from the tile palette object
			m_RenderState.texture = ResourceManager::GetResourceHandle<sf::Texture>(tilemap->PalettePtr->GetTexture());
			m_RenderState.transform = transform->GetLocalToWorldTransformMatrix();
			m_RenderState.shader = shader;

			{
				ZoneScoped;
				ZoneName("Draw", 4);
				m_RenderWindow->draw(tilemap->Geometry, m_RenderState);
			}
		}
	}

}

