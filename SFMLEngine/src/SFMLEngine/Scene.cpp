#include "Scene.h"


namespace SFMLEngine {

	Scene::Scene()
	{
		m_EntityRegistry = new std::set<Entity>;
	}

	Scene::~Scene()
	{
		delete m_EntityRegistry;
	}

	void Scene::Init(Coordinator* coordinator, std::shared_ptr<ScriptableEntitySystem> scriptableEntitySystem)
	{
		m_Coordinator = coordinator;
		m_ScriptableEntitySystem = scriptableEntitySystem;
	}

	void Scene::Destroy()
	{
		for (auto entity : *m_EntityRegistry)
		{
			m_Coordinator->DestroyEntity(entity);
		}
		m_EntityRegistry->clear();
	}
	

	Entity Scene::CreateEntity() 
	{
		auto newEntity = m_Coordinator->CreateEntity();
		m_EntityRegistry->insert(newEntity);

		return newEntity;
	}

	void Scene::DestroyEntity(Entity entity) 
	{ 
		m_EntityRegistry->erase(entity);
		m_Coordinator->DestroyEntity(entity); 
	}

}