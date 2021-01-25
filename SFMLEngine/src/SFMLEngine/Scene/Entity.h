#pragma once

#include "Component.h"

#include <vector>

namespace SFMLEngine {

	class Entity
	{
	public:
		Entity(const std::string& name);
		~Entity();

		void AddComponent(Component::ComponentType type);
		bool HasComponent(Component::ComponentType type);
		const Component& GetComponent(Component::ComponentType type);
		void RemoveComponent(Component::ComponentType type);

	private:
		std::string m_Name;

		std::vector<Component*> m_Components;
	};

}
