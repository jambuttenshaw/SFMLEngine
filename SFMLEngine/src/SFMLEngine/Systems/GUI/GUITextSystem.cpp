#include "GUITextSystem.h"

#include "SFMLEngine/ECS/Components/GUI/GUITransform.h"
#include "SFMLEngine/ECS/Components/GUI/GUIText.h"

namespace SFMLEngine {

	void GUITextSystem::Init(Coordinator* coordinator, sf::RenderWindow* window)
	{
		m_Coordinator = coordinator;
		m_RenderWindow = window;
	}

	void GUITextSystem::EntityAddedToSystem(Entity entity)
	{
	}

	void GUITextSystem::Render()
	{
		ZoneScoped;
		sf::RenderStates renderState;

		for (auto& e : m_Entities)
		{
			GUIText& text = m_Coordinator->GetComponent<GUIText>(e);
			GUITransform& transform = m_Coordinator->GetComponent<GUITransform>(e);

			renderState.transform = transform.GetTransformMatrix();

			m_RenderWindow->draw(text.GetTextObject(), renderState);
		}
	}

}
