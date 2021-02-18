#include "Scene.h"

#include "SFMLEngine/ECS/Components/Identity.h"
#include "LayerManager.h"


namespace SFMLEngine {

	Scene::Scene()
	{
		m_EntityRegistry = new std::set<Entity>;
	}

	Scene::~Scene()
	{
		delete m_EntityRegistry;
	}

	void Scene::Init(Coordinator* coordinator, std::shared_ptr<ScriptableEntitySystem> scriptableEntitySystem, std::shared_ptr<IdentitySystem> identitySystem)
	{
		m_Coordinator = coordinator;
		m_ScriptableEntitySystem = scriptableEntitySystem;
		m_IdentitySystem = identitySystem;
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
		std::string name = "Entity " + std::to_string(newEntity);
		Identity id{ name };
		AddComponent(newEntity, id);

		return newEntity;
	}

	Entity Scene::CreateEntity(const std::string& name, const std::string& tag, const std::string& layerName)
	{
		auto newEntity = m_Coordinator->CreateEntity();
		m_EntityRegistry->insert(newEntity);

		// add an identity component
		Identity id{ name, tag, LayerManager::CreateLayer(layerName) };
		AddComponent(newEntity, id);

		return newEntity;
	}

	void Scene::DestroyEntity(Entity entity) 
	{ 
		m_EntityRegistry->erase(entity);
		m_Coordinator->DestroyEntity(entity); 
	}

}