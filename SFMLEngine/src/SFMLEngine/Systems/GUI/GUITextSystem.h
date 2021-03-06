#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/Coordinator.h"


namespace SFMLEngine {

	class GUITextSystem : public System
	{
	public:
		GUITextSystem() = default;
		~GUITextSystem() = default;

		void Init(Coordinator* coordinator, sf::RenderWindow* window);

		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override {}

		void Render();

	private:
		Coordinator* m_Coordinator = nullptr;
		sf::RenderWindow* m_RenderWindow = nullptr;
	};

}
