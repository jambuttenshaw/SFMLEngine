#include "Entity.h"


namespace SFMLEngine {

	Entity::Entity(const std::string& name)
		: m_Name(name)
	{
	}

	Entity::~Entity()
	{
		for (Component* c : m_Components)
		{
			delete c;
		}
		m_Components.clear();
	}

	void Entity::AddComponent(Component::ComponentType type)
	{
		m_Components.push_back(Component::Create(type));
	}

	bool Entity::HasComponent(Component::ComponentType type)
	{
		for (Component* c : m_Components)
		{
			if (c->GetType() == type)
				return true;
		}
		return false;
	}

	Component* Entity::GetComponent(Component::ComponentType type)
	{
		for (Component* c : m_Components)
		{
			if (c->GetType() == type)
				return c;
		}
	}

	void Entity::RemoveComponent(Component::ComponentType type)
	{
		int toRemove = 0;
		int i = 0;
		for (Component* c : m_Components)
		{
			if (c->GetType() == type)
				break;
			i++;
		}
		m_Components.erase(m_Components.begin() + i);
	}

}