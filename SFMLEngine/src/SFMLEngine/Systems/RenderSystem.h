#pragma once

#include <SFML/Graphics.hpp>

#include "../ECS/Coordinator.h"
#include "../ECS/Components.h"

#include "../ResourceManagement/ResourceManager.h"

#include <algorithm>

namespace SFMLEngine {

	class RenderSystem : public System
	{
	public:
		RenderSystem() = default;
		~RenderSystem() = default;
		
		void Init(Coordinator* coordinator, sf::RenderTarget* target)
		{
			m_Coordinator = coordinator;
			m_RenderTarget = target;
		}

		void EntityAddedToSystem(Entity entity) override
		{
			auto& sRenderer = m_Coordinator->GetComponent<SpriteRenderer>(entity);
			sRenderer.Sprite.setTexture(*ResourceManager::GetResourceHandle<sf::Texture>(sRenderer.TextureHandle));
		}

		void Update()
		{
			for (auto const& entity : m_Entities)
			{
				auto& sRenderer = m_Coordinator->GetComponent<SpriteRenderer>(entity);
				auto& transform = m_Coordinator->GetComponent<Transform>(entity);

				sRenderer.Sprite.setPosition(transform.Position);
				sRenderer.Sprite.setRotation(transform.Rotation);
				sRenderer.Sprite.setScale(transform.Scale);
			}
		}

		void Render()
		{
			auto components = m_Coordinator->GetComponents<SpriteRenderer>(m_Entities);

			// sort components by the draw order
			std::sort(components.begin(), components.end(),
				[](const SpriteRenderer& a, const SpriteRenderer& b) -> bool
				{ return a.OrderInLayer < b.OrderInLayer; });

			for (const auto& c : components)
			{
				m_RenderTarget->draw(c.Sprite);
			}
		}

	private:
		Coordinator* m_Coordinator = nullptr;
		sf::RenderTarget* m_RenderTarget = nullptr;
	};

}
