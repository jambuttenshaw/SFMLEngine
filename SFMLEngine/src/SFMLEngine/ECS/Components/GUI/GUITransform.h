#pragma once

#include <SFML/Graphics.hpp>
#include "SFMLEngine/ECS/System.h"
#include "SFMLEngine/Math.h"

#include "GUIElement.h"


namespace SFMLEngine {

	/*
	A GUITransform is a seperate type of transform that positions objects in the HUD view
	It is a relative positioning system, that takes into account the size of the display
	to calculate the absolute position of the object on screen
	*/
	class GUITransform
	{
	public:

		friend class System;

		enum class Anchor
		{
			Left, Top, Middle, Right, Bottom
		};


		GUITransform()
			: m_ElementType(GUIElementType::Invalid)
		{}

		GUITransform(const sf::Vector2f& pos, GUIElementType elementType)
			: m_ElementType(elementType)
		{
			SetPosition(pos);
		}



		sf::Transform GetTransformMatrix()
		{
			sf::Transform t;
			t.translate(m_ScreenPosition);
			return t;
		}



		void SetPosition(const sf::Vector2f& pos)
		{
			m_Position = Math::Clamp(pos, { 0, 0 }, { 1, 1 });
			m_Modified = true;
		}
		const sf::Vector2f& GetPosition() { return m_Position; }

		void SetScreenPosition(const sf::Vector2f& pos) { m_ScreenPosition = pos; }
		const sf::Vector2f& GetScreenPosition() { return m_ScreenPosition; }


		void SetHorizontalAnchor(Anchor a)
		{
			if (a == Anchor::Top || a == Anchor::Bottom)
			{
				LOG_CORE_WARN("Cannot set horizontal anchor to Top or Bottom!");
				return;
			}
			m_HorizontalAnchor = a;

			m_Modified = true;
		}
		Anchor GetHorizontalAnchor() { return m_HorizontalAnchor; }


		void SetVerticalAnchor(Anchor a)
		{
			if (a == Anchor::Left || a == Anchor::Right)
			{
				LOG_CORE_WARN("Cannot set Vertical anchor to Left or Right!");
				return;
			}
			m_VerticalAnchor = a;

			m_Modified = true;
		}
		Anchor GetVerticalAnchor() { return m_VerticalAnchor; }


		GUIElementType GetElementType() { return m_ElementType; }


	private:
		// it is important that modified is marked at true on startup
		// so the system recalculates the screen position
		bool m_Modified = true;

		// the position is always normalized between 0 and 1
		// where 0,0 is the topleft of the screen and 1,1 is the bottomright
		sf::Vector2f m_Position;
		sf::Vector2f m_ScreenPosition;

		// anchors say which part of the object is positioned at m_Position
		// for example, anchors of Left, Top will position the topleft at m_Position
		// and anchors of Middle, Middle will centre the object over m_Position
		Anchor m_HorizontalAnchor = Anchor::Left;
		Anchor m_VerticalAnchor = Anchor::Top;


		// what type of gui object does this transform represent?
		GUIElementType m_ElementType;

	};

}
