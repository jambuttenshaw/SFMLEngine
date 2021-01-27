#pragma once

#include <SFML/Graphics.hpp>

#include "../ECS/Coordinator.h"
#include "../ECS/Components.h"

namespace SFMLEngine {

	class RenderSystem : public System
	{
	public:
		RenderSystem() = default;
		
		void Init(Coordinator* coordinator, sf::RenderTarget* target)
		{
			m_Coordinator = coordinator;
			m_RenderTarget = target;
		}

		void Render()
		{
			for (auto const& entity : m_Entities)
			{
				auto& sRenderer = m_Coordinator->GetComponent<SpriteRenderer>(entity);

				m_RenderTarget->draw(sRenderer.Sprite);
			}
		}

	private:
		Coordinator* m_Coordinator = nullptr;
		sf::RenderTarget* m_RenderTarget = nullptr;
	};

}
