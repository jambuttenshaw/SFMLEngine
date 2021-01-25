#include "Scene.h"

namespace SFMLEngine {

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
		for (Entity* e : m_Entities)
		{
			delete e;
		}
		m_Entities.clear();
	}

	Entity* Scene::CreateEntity()
	{
		Entity* newEntity = new Entity("New Entity " + m_Entities.size());
		newEntity->AddComponent(Component::ComponentType::Transform);
		return newEntity;
	}

	Component* Scene::FindObjectOfType(Component::ComponentType type)
	{
		for (Entity* e : m_Entities)
		{
			if (e->HasComponent(type))
				return e->GetComponent(type);
		}
		return nullptr;
	}

	const std::vector<Component*> Scene::FindObjectsOfType(Component::ComponentType type)
	{
		std::vector<Component*> objects;
		for (Entity* e : m_Entities)
		{
			if (e->HasComponent(type))
			{
				objects.push_back(e->GetComponent(type));
			}
		}

		return objects;
	}

	void Scene::RenderScene()
	{
		// lots to be done here
	}

}
