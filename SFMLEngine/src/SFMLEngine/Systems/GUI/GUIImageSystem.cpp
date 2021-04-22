#include "GUIImageSystem.h"


#include "SFMLEngine/ECS/Components/GUI/GUIImage.h"
#include "SFMLEngine/ECS/Components/GUI/GUITransform.h"

namespace SFMLEngine {

	void GUIImageSystem::Init(Coordinator* coordinator, sf::RenderWindow* window)
	{
		m_Coordinator = coordinator;
		m_RenderWindow = window;
	}

	void GUIImageSystem::EntityAddedToSystem(Entity entity)
	{
		m_SortedEntities.push_back(entity);

		SortEntities();
	}

	void GUIImageSystem::EntityRemovedFromSystem(Entity entity)
	{
		size_t index = 0;
		for (Entity e : m_SortedEntities)
		{
			if (e == entity)
				break; // each entity is unique so we dont have to worry about multiple entries
			index++;
		}
		m_SortedEntities.erase(m_SortedEntities.begin() + index);
	}

	void GUIImageSystem::SortEntities()
	{
		std::sort(m_SortedEntities.begin(), m_SortedEntities.end(),
			[this](const Entity& a, const Entity& b) -> bool
			{
				return m_Coordinator->GetComponent<GUIImage>(a).GetRenderLayer() < m_Coordinator->GetComponent<GUIImage>(b).GetRenderLayer();
			});
	}

	void GUIImageSystem::Render()
	{
		ZoneScoped;
		sf::RenderStates renderState;

		// iterate through the sorted container so they get drawn in the correct order
		for (auto& e : m_SortedEntities)
		{
			GUIImage& image = m_Coordinator->GetComponent<GUIImage>(e);
			GUITransform& transform = m_Coordinator->GetComponent<GUITransform>(e);

			renderState.transform = transform.GetTransformMatrix();
			m_RenderWindow->draw(image.GetSpriteObject(), renderState);
		}
	}

}