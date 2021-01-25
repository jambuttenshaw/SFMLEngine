#pragma once

#include "Entity.h"

namespace SFMLEngine {

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity* CreateEntity();

		std::vector<Entity*> GetEntitiesOfType(Component::ComponentType type);

		// some kind of render scene function
		void RenderScene();


	private:
		// scene needs to be aware of the window
		// to render stuff to
	};

}