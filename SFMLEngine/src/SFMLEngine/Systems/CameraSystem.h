#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/Coordinator.h"

namespace SFMLEngine {

	class CameraSystem : public System
	{
	public:
		CameraSystem() = default;
		~CameraSystem() = default;

		void Init(Coordinator* coordinator);

		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override {}

		void Update();

		void WindowResized(const sf::Vector2f& newSize);
		sf::View GetMainCameraView();

	private:

		Coordinator* m_Coordinator = nullptr;

		Entity m_MainCamera;
	};

}
