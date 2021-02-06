#include "GUISystem.h"


namespace SFMLEngine {

	void GUISystem::Init(Coordinator* coordinator, sf::RenderWindow* window)
	{
		m_Coordinator = coordinator;
		m_RenderWindow = window;
	}

	void GUISystem::EntityAddedToSystem(Entity entity)
	{
		auto& text = m_Coordinator->GetComponent<Text>(entity);
		if (text.Font != NULL_RESOURCE_ID)
		{
			text.TextObject.setFont(*ResourceManager::GetResourceHandle<sf::Font>(text.Font));
		}
		else
		{
			LOG_CORE_WARN("No font was supplied for entity {0}.", entity);
		}
		text.TextObject.setString(text.String);
		text.TextObject.setCharacterSize(text.FontSize);
		text.TextObject.setFillColor(text.TextColor);
	}

	void GUISystem::Update()
	{
		for (auto& e : m_Entities)
		{
			Text& text = m_Coordinator->GetComponent<Text>(e);
			if (ComponentModified(text))
			{
				text.TextObject.setString(text.String);
				text.TextObject.setCharacterSize(text.FontSize);
				text.TextObject.setFillColor(text.TextColor);

				ResetModified(text);
			}
		}
	}

	void GUISystem::Render()
	{
		sf::RenderStates renderState;

		for (auto& e : m_Entities)
		{
			Text& text = m_Coordinator->GetComponent<Text>(e);
			Transform& transform = m_Coordinator->GetComponent<Transform>(e);

			renderState.transform = transform.GetTransformMatrix();

			m_RenderWindow->draw(text.TextObject, renderState);
		}
	}

}