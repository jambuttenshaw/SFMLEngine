#pragma once

#include "Entity.h"

namespace SFMLEngine {

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity* CreateEntity();

		const Component& FindObjectOfType(Component::ComponentType type);
		const std::vector<const Component&>& FindObjectsOfType(Component::ComponentType type);

		// some kind of render scene function
		void RenderScene();


	private:
		// scene needs to be aware of the window
		// to render stuff to


		// the scene has a list of all the entities present in it
		std::vector<Entity*> m_Entities;

	};

}