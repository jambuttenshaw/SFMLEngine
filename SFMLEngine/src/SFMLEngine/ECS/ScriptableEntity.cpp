#include "ScriptableEntity.h"
#include "SFMLEngine/Scene.h"

namespace SFMLEngine {

	const std::vector<Entity>& ScriptableEntity::GetEntitiesWithTag(const std::string& tag)
	{ 
		return m_SceneHandle->GetEntitiesWithTag(tag);
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
