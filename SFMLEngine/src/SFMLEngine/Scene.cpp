#include "Scene.h"

#include "SFMLEngine/ECS/Components/Identity.h"
#include "LayerManager.h"

#include "SFMLEngine/DebugTools.h"


namespace SFMLEngine {

	Scene::Scene()
	{
		m_EntityRegistry = new std::set<Entity>;
	}

	Scene::~Scene()
	{
		delete m_EntityRegistry;
	}

	void Scene::Init(Coordinator* coordinator,
		std::shared_ptr<ScriptableEntitySystem> scriptableEntitySystem,
		std::shared_ptr<PhysicsSystem> physicsSystem,
		std::shared_ptr<IdentitySystem> identitySystem)
	{
		m_Coordinator = coordinator;
		m_ScriptableEntitySystem = scriptableEntitySystem;
		m_PhysicsSystem = physicsSystem;
		m_IdentitySystem = identitySystem;
	}

	void Scene::Destroy()
	{
		// iterate backwards through the entity registry
		for (auto& entity : *m_EntityRegistry)
		{
			DeleteEntityData(entity);
		}
		m_EntityRegistry->clear();
	}

	void Scene::DestroyAllPending()
	{
		for (auto const& entity : m_EntitiesToDestroy)
		{
			DestroyEntityImmediate(entity);
		}
		m_EntitiesToDestroy.clear();
	}
	

	Entity Scene::CreateEntity() 
	{
		auto newEntity = m_Coordinator->CreateEntity();
		m_EntityRegistry->insert(newEntity);

		// add an identity component
		std::string name = "Entity " + std::to_string(newEntity);
		Identity id{ name };
		id.SetLayer(LayerManager::LayerFromString("Default"));
		AddComponent(newEntity, id);

		return newEntity;
	}

	Entity Scene::CreateEntity(const std::string& name, const std::string& tag, const std::string& layerName)
	{
		auto newEntity = m_Coordinator->CreateEntity();
		m_EntityRegistry->insert(newEntity);

		// add an identity component
		Identity id{ name, tag, LayerManager::LayerFromString(layerName) };
		AddComponent(newEntity, id);

		return newEntity;
	}

	void Scene::DestroyEntity(Entity entity) 
	{ 
		m_EntitiesToDestroy.push_back(entity);
	}

	void Scene::DestroyEntityImmediate(Entity entity)
	{
		DeleteEntityData(entity);
		m_EntityRegistry->erase(entity);
	}


	void Scene::SetEntityName(Entity entity, const std::string& name)
	{
		Identity& identityComponent = GetComponent<Identity>(entity);
		identityComponent.SetName(name);
	}
	const std::string& Scene::GetEntityName(Entity entity)
	{
		return GetComponent<Identity>(entity).GetName();
	}


	void Scene::SetEntityTag(Entity entity, const std::string& tag)
	{
		Identity& identityComponent = GetComponent<Identity>(entity);
		identityComponent.SetTag(tag);
		m_IdentitySystem->UpdateTag(entity);
	}
	const std::string& Scene::GetEntityTag(Entity entity)
	{
		return GetComponent<Identity>(entity).GetTag();
	}

	void Scene::SetEntityLayer(Entity entity, const std::string& layerName)
	{
		Identity& identityComponent = GetComponent<Identity>(entity);
		identityComponent.SetLayer(LayerManager::LayerFromString(layerName));
	}
	const std::string& Scene::GetEntityLayer(Entity entity)
	{
		return LayerManager::GetLayerName(GetComponent<Identity>(entity).GetLayer());
	}



	void Scene::DeleteEntityData(Entity entity)
	{
		// if this entity is involved in any physics callbacks,
		// these need to be sent before the entity destruction
		// to guarantee that the data is valid when the callback is sent
		m_PhysicsSystem->EntityDeleted(entity);

		m_Coordinator->DestroyEntity(entity);
	}
}