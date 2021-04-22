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



		inline sf::Transform GetTransformMatrix() const
		{
			sf::Transform t;
			t.translate(static_cast<sf::Vector2f>(m_ScreenPosition));
			return t;
		}



		inline void SetPosition(const sf::Vector2f& pos)
		{
			m_Position = Math::Clamp(pos, { 0, 0 }, { 1, 1 });
			m_Modified = true;
		}
		inline const sf::Vector2f& GetPosition() const { return m_Position; }

		inline void SetScreenPosition(const sf::Vector2i& pos) { m_ScreenPosition = pos; }
		inline const sf::Vector2i& GetScreenPosition() const { return m_ScreenPosition; }

		inline void SetScale(const sf::Vector2f& scale) { m_Scale = scale; }
		inline const sf::Vector2f& GetScale() const { return m_Scale; }

		inline void SetScaleWithScreen(bool flag) { m_ScaleWithScreen = flag; }
		inline bool GetScaleWithScreen() const { return m_ScaleWithScreen; }


		sf::Vector2f GetTopLeft(const sf::Vector2f& elemSize) const;
		sf::Vector2f GetTopRight(const sf::Vector2f& elemSize) const;
		sf::Vector2f GetBottomLeft(const sf::Vector2f& elemSize) const;
		sf::Vector2f GetBottomRight(const sf::Vector2f& elemSize) const;

		void SetHorizontalAnchor(Anchor a);
		inline Anchor GetHorizontalAnchor() const { return m_HorizontalAnchor; }


		void SetVerticalAnchor(Anchor a);
		inline Anchor GetVerticalAnchor() const { return m_VerticalAnchor; }


		inline GUIElementType GetElementType() const { return m_ElementType; }


	private:
		// it is important that modified is marked at true on startup
		// so the system recalculates the screen position
		bool m_Modified = true;

		// the position is always normalized between 0 and 1
		// where 0,0 is the topleft of the screen and 1,1 is the bottomright
		sf::Vector2f m_Position;
		sf::Vector2i m_ScreenPosition;

		sf::Vector2f m_Scale{ 1, 1 };
		bool m_ScaleWithScreen = true;

		// anchors say which part of the object is positioned at m_Position
		// for example, anchors of Left, Top will position the topleft at m_Position
		// and anchors of Middle, Middle will centre the object over m_Position
		Anchor m_HorizontalAnchor = Anchor::Left;
		Anchor m_VerticalAnchor = Anchor::Top;


		// what type of gui object does this transform represent?
		GUIElementType m_ElementType;

	};

}
