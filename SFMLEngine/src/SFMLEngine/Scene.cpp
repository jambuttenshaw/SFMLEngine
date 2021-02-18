#include "Scene.h"

#include "SFMLEngine/ECS/Components/Identity.h"


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

		// add an identity component
		Identity id{ "Entity " + std::to_string(newEntity) };
		AddComponent(newEntity, id);

		return newEntity;
	}

	Entity Scene::CreateEntity(const std::string& name, const std::string& tag, Layer layer)
	{
		auto newEntity = m_Coordinator->CreateEntity();
		m_EntityRegistry->insert(newEntity);

		// add an identity component
		Identity id{ name, tag, layer };
		AddComponent(newEntity, id);

		return newEntity;
	}

	void Scene::DestroyEntity(Entity entity) 
	{ 
		m_EntityRegistry->erase(entity);
		m_Coordinator->DestroyEntity(entity); 
	}

}