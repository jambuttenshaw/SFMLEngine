#pragma once

#include <SFML/Graphics.hpp>

#include "../ECS/Coordinator.h"


namespace SFMLEngine {

	class GUISystem : public System
	{
	public:
		GUISystem() = default;
		~GUISystem() = default;

		void Init(Coordinator* coordinator, sf::RenderWindow* window);

		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override {}

		void Update();
		void Render();

	private:
		Coordinator* m_Coordinator;
		sf::RenderWindow* m_RenderWindow;
	};

}
