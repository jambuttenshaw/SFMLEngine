#pragma once

#include "SFMLEngine/ECS/Coordinator.h"
#include "SFMLEngine/ECS/Components/NativeScripts.h"
#include "SFMLEngine/ECS/System.h"
#include "SFMLEngine/Timestep.h"

#include <type_traits>



/*
The scriptable entity system manages adding native script objects onto entities
*/


namespace SFMLEngine {

	class ScriptableEntitySystem : public System
	{
	public:
		ScriptableEntitySystem() = default;
		
		~ScriptableEntitySystem()
		{
			// the scriptable entity system is responsible for deleting the
			// instances of the scripts that have been created
			for (auto const& entity : m_Entities)
			{
				auto& nativeScriptComponent = m_Coordinator->GetComponent<NativeScripts>(entity);
				for (auto& script : nativeScriptComponent.GetScripts())
				{
					// delete the script object that is held in the map
					delete script.second;
				}
				nativeScriptComponent.ClearScripts();
			}
		}

		void Init(Coordinator* coordinator) 
		{
			m_Coordinator = coordinator;
		}

		void EntityAddedToSystem(Entity entity) override {}
		void EntityRemovedFromSystem(Entity entity) override {}

		void Start()
		{
			for (auto const& entity : m_Entities)
			{
				auto& scriptComponent = m_Coordinator->GetComponent<NativeScripts>(entity);
				for (auto& script : scriptComponent.GetScripts())
					script.second->Start();
			}

			m_Started = true;
		}

		void NewSceneLoaded()
		{
			for (auto const& entity : m_Entities)
			{
				auto& scriptComponent = m_Coordinator->GetComponent<NativeScripts>(entity);
				for (auto& script : scriptComponent.GetScripts())
					script.second->OnSceneLoaded();
			}
		}

		bool GetStarted() { return m_Started; }
		void Reset() { m_Started = false; }
		void StartAllPending() 
		{
			for (ScriptableEntity* script : m_PendingStart)
				script->Start();
			m_PendingStart.clear();
		}
		void SetSceneLoading(bool loading) { m_SceneLoadInProgress = loading; }

		void Update(Timestep ts)
		{
			ZoneScoped;

			for (auto const& entity : m_Entities)
			{
				auto& scriptComponent = m_Coordinator->GetComponent<NativeScripts>(entity);
				for (auto& script : scriptComponent.GetScripts())
					script.second->Update(ts);
			}
		}

		template<typename T>
		T& AddNativeScript(Entity entity, void* scene)
		{
			static_assert(std::is_base_of<ScriptableEntity, T>::value, "Native scripts must inherit from Scriptable entity!");
			
			// check if the entity has a native scripts component
			// if it doesn't then one should be added
			// unfortunately we can't guarentee the component exists like in other systems
			// because native scripts aren't added through the usual AddComponent method
			if (!m_Coordinator->HasComponent<NativeScripts>(entity))
				m_Coordinator->AddComponent(entity, NativeScripts{});

			// get the component
			auto& nativeScriptComponent = m_Coordinator->GetComponent<NativeScripts>(entity);


			// get a unique identifier for this type of script
			const char* typeName = typeid(T).name();


			// make sure that this type of script hasn't already been added
			SFMLE_CORE_ASSERT(!nativeScriptComponent.HasScript(typeName), "Entity already contains script of this type!");


			// instantiate a new script
			T* newScript = new T;

			// cast it down to a ScriptableEntity* so we can use the base class interface
			// and since we static_assert'd that it inherits from ScriptableEntity, we can guarentee this will work
			ScriptableEntity* scriptableEntity = static_cast<ScriptableEntity*>(newScript);
			// set the handles to the script can interact with the engine
			scriptableEntity->SetSceneHandle(scene);
			scriptableEntity->SetEntityHandle(entity);

			// add this script into the map
			nativeScriptComponent.AddScript(std::make_pair(typeName, scriptableEntity));

			// if the script is created during the game then call the start immediately after its created
			if (m_Started)
			{
				if (m_SceneLoadInProgress)
					m_PendingStart.push_back(scriptableEntity);
				else
					scriptableEntity->Start();
			}

			// return the new script in case the user wants to interact with it immediately after its creation,
			// such as intializing data members of the ScriptableEntity
			return *newScript;
		}

		template<typename T>
		void RemoveNativeScript(Entity entity)
		{
			// get the unique id for this script
			const char* typeName = typeid(T).name();
			// get the scripts component
			auto& scripts = m_Coordinator->GetComponent<NativeScripts>(entity);

			// make sure the entity contains a script of this type
			SFMLE_CORE_ASSERT(scripts.HasScript(typeName),  "Entity did not have script attatched!");

			// then erase this script form the map
			scripts.DeleteScript(typeName);
		}

		template<typename T>
		T& GetNativeScript(Entity entity)
		{
			// get the unique id for this script
			const char* typeName = typeid(T).name();
			// get the scripts component
			auto& scripts = m_Coordinator->GetComponent<NativeScripts>(entity);

			// make sure the entity actually has this script attached
			SFMLE_CORE_ASSERT(scripts.HasScript(typeName), "Entity did not have script attatched!");

			// return a reference to the script, casted to its derived type
			return *static_cast<T*>(scripts.GetScript(typeName));
		}

	private:
		Coordinator* m_Coordinator = nullptr;

		// should probably be migrated to application
		bool m_Started = false;

		bool m_SceneLoadInProgress = false;
		std::vector<ScriptableEntity*> m_PendingStart;
	};

}
