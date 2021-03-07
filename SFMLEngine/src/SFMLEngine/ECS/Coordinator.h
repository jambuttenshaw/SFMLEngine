#pragma once

#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

namespace SFMLEngine {

	// the coordinator is the link between all the different managers 
	// that make up the ecs
	
	// it provides communication between the entity manager, systems manager, and component manager

	// a lot of its functionality is to just pass along data into the other managers
	// but makes the ECS far easier to use by providing a simple api

	class Coordinator
	{
	public:
		void Init()
		{
			// Create pointers to each manager
			m_ComponentManager = std::make_unique<ComponentManager>();
			m_EntityManager = std::make_unique<EntityManager>();
			m_SystemManager = std::make_unique<SystemManager>();
		}


		// Entity methods
		Entity CreateEntity()
		{
			return m_EntityManager->CreateEntity();
		}

		void DestroyEntity(Entity entity)
		{
			// we need to tell every system that an entity was destroyed
			// so all of its components can be deleted
			// and it can be removed from all of the systems
			m_SystemManager->EntityDestroyed(entity);
			m_ComponentManager->EntityDestroyed(entity);
			m_EntityManager->DestroyEntity(entity);
		}


		// Component methods
		template<typename T>
		void RegisterComponent()
		{
			m_ComponentManager->RegisterComponent<T>();
		}

		template<typename T>
		void AddComponent(Entity entity, const T& component)
		{
			// add the data in the component manager
			m_ComponentManager->AddComponent<T>(entity, component);

			// we need to change the signature of the entity to include this component
			auto signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentType<T>(), true);
			m_EntityManager->SetSignature(entity, signature);

			// and when the signature changes, its possible the entity will need to be added into more systems
			// so we tell the systems manager to re-evaluate what systems this entity is subscribed to
			m_SystemManager->EntitySignatureChanged(entity, signature);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			// delete the data for this component on this entity
			m_ComponentManager->RemoveComponent<T>(entity);

			// then we need to change the signature of the entity again
			// by resetting the bit that represents this component
			auto signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentType<T>(), false);
			m_EntityManager->SetSignature(entity, signature);

			// and now the signature has changed its possible this entity needs to be removed from
			// some systems, so we tell the systems manager to check that for us
			m_SystemManager->EntitySignatureChanged(entity, signature);
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			// get the component data
			ZoneScoped;
			return m_ComponentManager->GetComponent<T>(entity);
		}

		template<typename T>
		std::vector<T> GetComponents(std::set<Entity> entities)
		{
			// returns a vector of all components from the set of entities
			// all entities should have the specified component
			// but if this is being called from a system, that can be guarenteed
			std::vector<T> components;
			for (auto const& entity : entities)
			{
				components.push_back(m_ComponentManager->GetComponent<T>(entity));
			}
			return components;
		}

		template<typename T>
		bool HasComponent(Entity entity)
		{
			// check if an entity has a component
			return m_EntityManager->HasComponentInSignature(entity, m_ComponentManager->GetComponentType<T>());
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			// get the type of the component
			// this is the index of the bit that represents this component in a signature
			return m_ComponentManager->GetComponentType<T>();
		}


		// System methods
		template<typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			// register the system with the system manager
			return m_SystemManager->RegisterSystem<T>();
		}

		template<typename T>
		void SetSystemSignature(Signature signature)
		{
			// the system signature is what components an entity needs to have
			// to be subscribed to that system
			m_SystemManager->SetSignature<T>(signature);
		}

	private:
		std::unique_ptr<ComponentManager> m_ComponentManager;
		std::unique_ptr<EntityManager> m_EntityManager;
		std::unique_ptr<SystemManager> m_SystemManager;
	};

}
