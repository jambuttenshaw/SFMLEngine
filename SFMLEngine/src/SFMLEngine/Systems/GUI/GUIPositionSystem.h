#pragma once

#include <SFML/Graphics.hpp>
#include "SFMLEngine/ECS/Coordinator.h"


namespace SFMLEngine {

	class GUITransform;


	class GUIPositionSystem : public System
	{
	public:
		GUIPositionSystem() = default;
		~GUIPositionSystem() = default;

		void Init(Coordinator* coordinator, sf::RenderWindow* window);

		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override;

		// check if any of the entities have been modified
		void Update();

		// used when the window is resized, cause all gui will need repositioned
		void RecalculateAll();

	private:
		// calculates the new position of a single entity
		void UpdatePosition(Entity entity, GUITransform& guiTransform);

	private:
		Coordinator* m_Coordinator = nullptr;
		// this system needs to be able to get the side of the window
		sf::RenderWindow* m_Window = nullptr;

	};

}
