#include "ScriptableEntity.h"
#include "SFMLEngine/Scene.h"

namespace SFMLEngine {

	// any functions that need to interact with the scene need defined in the cpp file to avoid circular includes

	Entity ScriptableEntity::CreateEntity() { return m_SceneHandle->CreateEntity(); }
	void ScriptableEntity::Destroy() { m_SceneHandle->DestroyEntity(m_EntityHandle); }

	const std::vector<Entity>& ScriptableEntity::GetEntitiesWithTag(const std::string& tag)
	{ 
		return m_SceneHandle->GetEntitiesWithTag(tag);
	}

	const std::string& ScriptableEntity::GetEntityName(Entity entity)
	{
		return m_SceneHandle->GetEntityName(entity);
	}
	const std::string& ScriptableEntity::GetEntityTag(Entity entity)
	{
		return m_SceneHandle->GetEntityTag(entity);
	}
	const std::string& ScriptableEntity::GetEntityLayer(Entity entity)
	{
		return m_SceneHandle->GetEntityLayer(entity);
	}


	void ScriptableEntity::SetSceneHandle(void* scene)
	{
		m_SceneHandle = static_cast<Scene*>(scene);
	}

	void ScriptableEntity::SetEntityHandle(Entity entity)
	{
		m_EntityHandle = entity;
	}

}
