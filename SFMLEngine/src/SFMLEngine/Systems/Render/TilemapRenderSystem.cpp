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
		
		m_TilemapRenderers.insert(std::make_pair(entity, tRenderer));
		m_Tilemaps.insert(std::make_pair(entity, tilemap));
		m_Transforms.insert(std::make_pair(entity, transform));



		m_RenderLayers.insert(tRenderer->GetRenderLayer());

		// insert into materials map
		if (m_MaterialsMap.find(tRenderer->GetMaterialHandle()) == m_MaterialsMap.end())
		{
			m_MaterialsMap.insert(std::make_pair(tRenderer->GetMaterialHandle(), std::set<Entity>{ entity }));
		}
		else
		{
			m_MaterialsMap[tRenderer->GetMaterialHandle()].insert(entity);
		}
	}

	void TilemapRenderSystem::Update()
	{
		for (auto& entity : m_Entities)
		{
			TilemapRenderer* renderer = m_TilemapRenderers[entity];
			if (ComponentModified(renderer))
			{
				// check to see if the material has been changed
				auto mat = renderer->GetMaterialHandle();
				if (m_MaterialsMap[mat].find(entity) == m_MaterialsMap[mat].end())
				{
					// the material has been changed

					// remove the entity from its material
					// we dont know the old material of the entity,
					// so we just have to search and erase
					for (auto& material : m_MaterialsMap)
					{
						material.second.erase(entity);
					}


					// insert the entity to the new material
					if (m_MaterialsMap.find(mat) == m_MaterialsMap.end())
					{
						m_MaterialsMap.insert(std::make_pair(mat, std::set<Entity>{ entity }));
					}
					else
					{
						m_MaterialsMap[mat].insert(entity);
					}
				}

				ResetModified(renderer);
			}
		}
	}

	void TilemapRenderSystem::EntityRemovedFromSystem(Entity entity)
	{
		auto mat = m_TilemapRenderers[entity]->GetMaterialHandle();
		m_MaterialsMap[mat].erase(entity);
		if (m_MaterialsMap[mat].empty())
			m_MaterialsMap.erase(mat);

		m_TilemapRenderers.erase(entity);
		m_Tilemaps.erase(entity);
		m_Transforms.erase(entity);
	}

	void TilemapRenderSystem::Render(ResourceID material, int renderLayer)
	{
		ZoneScoped;

		if (m_MaterialsMap[material].empty()) return;

		for (const auto& entity : m_MaterialsMap[material])
		{
			ZoneScoped;
			ZoneName("DrawTilemap", 11);

			TilemapRenderer* tilemapRenderer = m_TilemapRenderers[entity];
			if (tilemapRenderer->GetRenderLayer() != renderLayer) continue;

			Tilemap* tilemap = m_Tilemaps[entity];
			Transform* transform = m_Transforms[entity];

			// get a pointer to the shader
			sf::Shader* shader = tilemapRenderer->GetMaterial()->GetShaderPtr();


			// set shader uniforms
			// if its a lit material set the normal map and rotation to be applied to normals
			if (tilemapRenderer->GetMaterial()->IsLit())
			{
				ZoneScoped;
				ZoneName("SetLighting", 11);

				// set the normal map to the normal map texture in the tile palette object
				shader->setUniform("u_NormalMap", *ResourceManager::GetResourceHandle<sf::Texture>(tilemap->GetPalette()->GetNormalMap()));

				// rotation value is used to compute transformed normals so lighting is correct for rotated sprites
				// requires negated because of the y axis being flipped
				shader->setUniform("u_Rotation", fmodf(-transform->GetRotation(), 360) * Math::DEG_TO_RAD);

			}

			// set up the render state
			// use the texture from the tile palette object
			m_RenderState.texture = ResourceManager::GetResourceHandle<sf::Texture>(tilemap->GetPalette()->GetTexture());
			m_RenderState.transform = transform->GetLocalToWorldTransformMatrix();
			m_RenderState.shader = shader;

			{
				ZoneScoped;
				ZoneName("Draw", 4);
				m_RenderWindow->draw(tilemap->GetGeometry(), m_RenderState);
			}
		}
	}

}

