#include "ScriptableEntity.h"
#include "../Scene.h"

namespace SFMLEngine {

	void ScriptableEntity::SetSceneHandle(void* scene)
	{
		m_SceneHandle = static_cast<Scene*>(scene);
	}

	void ScriptableEntity::SetEntityHandle(Entity entity)
	{
		m_EntityHandle = entity;
	}

}
