#pragma once

#include "../Constants.h"


namespace SFMLEngine {

	class Scene;

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {};

		virtual void Awake() {};
		virtual void Start() {};

		virtual void Update(float timestep) {};

		// components
		template<typename T>
		T& AddComponent(T component) { return m_SceneHandle->AddComponent(m_EntityHandle, component); }
		template<typename T>
		void RemoveComponent() { m_SceneHandle->RemoveComponent<T>(m_EntityHandle); }
		template<typename T>
		T& GetComponent() { return m_SceneHandle->GetComponent<T>(m_EntityHandle); }

		// native scripting
		template<typename T>
		T& AddNativeScript() { return m_SceneHandle->AddNativeScript<T>(m_EntityHandle); }
		template<typename T>
		void RemoveNativeScript() { m_SceneHandle->RemoveNativeScript<T>(m_EntityHandle); }
		template<typename T>
		T& GetNativeScript() { return m_SceneHandle->GetNativeScript<T>(m_EntityHandle); }


		void SetSceneHandle(void* scene);
		void SetEntityHandle(Entity entity);

	private:
		Scene* m_SceneHandle = nullptr;
		Entity m_EntityHandle;
	};

}
