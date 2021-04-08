#include "SpriteRenderSystem.h"

#include "SFMLEngine/Math.h"
#include "SFMLEngine/Renderer/Renderer.h"


namespace SFMLEngine {

	void SpriteRenderSystem::Init(Coordinator* coordinator, sf::RenderWindow* window)
	{
		m_Coordinator = coordinator;
		m_RenderWindow = window;
	}


	void SpriteRenderSystem::EntityAddedToSystem(Entity entity)
	{
		SpriteRenderer* sRenderer = &m_Coordinator->GetComponent<SpriteRenderer>(entity);
		Transform* transform = &m_Coordinator->GetComponent<Transform>(entity);

		m_SpriteRenderers.insert(std::make_pair(entity, sRenderer));
		m_Transforms.insert(std::make_pair(entity, transform));

		// insert into render layers map
		m_RenderLayers.insert(sRenderer->GetRenderLayer());

		// insert into materials map
		if (m_MaterialsMap.find(sRenderer->GetMaterialHandle()) == m_MaterialsMap.end())
		{
			m_MaterialsMap.insert(std::make_pair(sRenderer->GetMaterialHandle(), std::set<Entity>{ entity }));
		}
		else
		{
			m_MaterialsMap[sRenderer->GetMaterialHandle()].insert(entity);
		}
	}

	void SpriteRenderSystem::EntityRemovedFromSystem(Entity entity)
	{
		auto mat = m_SpriteRenderers[entity]->GetMaterialHandle();
		m_MaterialsMap[mat].erase(entity);
		if (m_MaterialsMap[mat].empty())
			m_MaterialsMap.erase(mat);

		m_SpriteRenderers.erase(entity);
		m_Transforms.erase(entity);
	}

	void SpriteRenderSystem::Update()
	{
		for (auto& entity : m_Entities)
		{
			SpriteRenderer* renderer = m_SpriteRenderers[entity];
			if (ComponentModified(renderer))
			{
				// check to see if the material has been changed
				auto mat = renderer->GetMaterialHandle();
				if (m_MaterialsMap[mat].find(entity) == m_MaterialsMap[mat].end())
				{
					// the material has been changed
					// remove the entity from its material
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

	void SpriteRenderSystem::Render(ResourceID material, int renderLayer)
	{
		ZoneScoped;

		if (m_MaterialsMap[material].empty()) return;

		for (const auto& entity : m_MaterialsMap[material])
		{
			ZoneScoped;
			ZoneName("DrawSprite", 10);
			SpriteRenderer* sR = m_SpriteRenderers[entity];
			if (sR->GetRenderLayer() != renderLayer) continue;

			Transform* t = m_Transforms[entity];
			
			sf::Shader* shader = sR->GetMaterial()->GetShaderPtr();
			if (sR->GetMaterial()->IsLit())
			{
				ZoneScoped;
				ZoneName("SetLight", 8);
				shader->setUniform("u_NormalMap", *sR->GetNormalMap());
					
				// rotation value is used to compute transformed normals so lighting is correct for rotated sprites
				// requires negated because of the y axis being flipped
				shader->setUniform("u_Rotation", (sR->GetFlippedNormals() ? -1 : 1) * fmodf(-t->Rotation, 360) * Math::DEG_TO_RAD);
			}
				
			// create a transform
			if (Math::SquareMagnitude(sR->GetOffset()))
			{
				sf::Transform transform;
				transform.translate(sR->GetOffset());
				m_RenderState.transform = transform.combine(t->GetLocalToWorldTransformMatrix());
			}
			else
			{
				m_RenderState.transform = t->GetLocalToWorldTransformMatrix();
			}
			m_RenderState.shader = shader;
				
			{
				ZoneScoped;
				ZoneName("Draw", 4);
				m_RenderWindow->draw(sR->GetSpriteObject(), m_RenderState);
			}
		}
		
	}
}
