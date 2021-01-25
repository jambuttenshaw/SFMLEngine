#pragma once

#include "Component.h"

#include <vector>

namespace SFMLEngine {

	class Entity
	{
	public:
		Entity();
		~Entity();

		void AddComponent(Component::ComponentType type);
		bool HasComponent(Component::ComponentType type);
		const Component& GetComponent(Component::ComponentType type);
		void RemoveComponent(Component::ComponentType type);

	private:
		std::vector<Component*> m_Components;
	};

}
