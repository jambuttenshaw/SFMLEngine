#include "GUIPositionSystem.h"

#include "SFMLEngine/ECS/Components/GUI/GUITransform.h"
#include "SFMLEngine/ECS/Components/GUI/GUIImage.h"
#include "SFMLEngine/ECS/Components/GUI/GUIText.h"


namespace SFMLEngine {

	void GUIPositionSystem::Init(Coordinator* coordinator, sf::RenderWindow* window)
	{
		m_Coordinator = coordinator;
		m_Window = window;

		m_WindowSize = static_cast<sf::Vector2f>(m_Window->getSize());
		m_InitialWindowSize = m_WindowSize;
	}

	void GUIPositionSystem::EntityAddedToSystem(Entity entity)
	{
	}

	void GUIPositionSystem::EntityRemovedFromSystem(Entity entity)
	{
	}

	void GUIPositionSystem::Update()
	{
		for (auto const& entity : m_Entities)
		{
			auto& guiTransform = m_Coordinator->GetComponent<GUITransform>(entity);
			if (ComponentModified(guiTransform))
			{
				UpdatePosition(entity, guiTransform);
				ResetModified(guiTransform);
			}
		}
	}

	void GUIPositionSystem::RecalculateAll()
	{
		m_WindowSize = static_cast<sf::Vector2f>(m_Window->getSize());
		for (auto const& entity : m_Entities)
		{
			UpdatePosition(entity, m_Coordinator->GetComponent<GUITransform>(entity));
		}
	}

	void GUIPositionSystem::UpdatePosition(Entity entity, GUITransform& guiTransform)
	{
		// calculate the absolute position of a object

		
		// the object also gets scaled so its relative size stays consistent
		sf::Vector2f scale = { m_WindowSize.x * guiTransform.GetScale().x / m_InitialWindowSize.x,
							   m_WindowSize.y * guiTransform.GetScale().y / m_InitialWindowSize.y };



		// first of all, we need to get the size of the element
		GUIElement* elem;
		switch (guiTransform.GetElementType())
		{
		case GUIElementType::Invalid:		SFMLE_CORE_ASSERT(0, "Invalid element type!");			break;
		case GUIElementType::Image:			elem = &m_Coordinator->GetComponent<GUIImage>(entity);	break;
		case GUIElementType::Text:			elem = &m_Coordinator->GetComponent<GUIText>(entity);	break;
		}

		sf::Vector2f size = elem->GetSize();
		size.x *= scale.x;
		size.y *= scale.y;

		// the offset applied to the object by the anchor value
		sf::Vector2f anchorOffset;
		switch (guiTransform.GetHorizontalAnchor())
		{
		case GUITransform::Anchor::Left:	anchorOffset.x = 0;				break;
		case GUITransform::Anchor::Middle:	anchorOffset.x = 0.5f * size.x; break;
		case GUITransform::Anchor::Right:	anchorOffset.x = size.x;		break;
		}
		switch (guiTransform.GetVerticalAnchor())
		{
		case GUITransform::Anchor::Top:		anchorOffset.y = 0;				break;
		case GUITransform::Anchor::Middle:	anchorOffset.y = 0.5f * size.y; break;
		case GUITransform::Anchor::Bottom:	anchorOffset.y = size.y;		break;
		}
		
		// then since the position of a gui element is between 0 and 1, it is effectively a fraction of the window size
		// with the anchor offset applied
		guiTransform.SetScreenPosition(
			static_cast<sf::Vector2i>(sf::Vector2f{
				(m_WindowSize.x * guiTransform.GetPosition().x) - anchorOffset.x,
				(m_WindowSize.y * guiTransform.GetPosition().y) - anchorOffset.y,
			})
		);

		elem->SetScale(scale);

	}

}
