#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/System.h"
#include "SFMLEngine/ResourceManagement/ResourceManager.h"
#include "GUIElement.h"


namespace SFMLEngine {

	class GUIText : public GUIElement
	{
	public:
		friend class System;

		

		GUIText()
			: m_Font(NULL_RESOURCE_ID), m_String(), m_FontSize(0), m_TextColor(sf::Color::White), m_TextObject()
		{}
		GUIText(ResourceID font, const std::string& textString, int fontSize, const sf::Color& textColor)
			: m_Font(font), m_String(textString), m_FontSize(fontSize), m_TextColor(textColor), m_TextObject()
		{
			m_TextObject.setFont(*ResourceManager::GetResourceHandle<sf::Font>(m_Font));
			m_TextObject.setString(m_String);
			m_TextObject.setCharacterSize(m_FontSize);
			m_TextObject.setFillColor(m_TextColor);
		}

		inline ResourceID GetFont() const { return m_Font; }
		inline void SetFont(ResourceID newFont)
		{ 
			m_Font = newFont;
			m_TextObject.setFont(*ResourceManager::GetResourceHandle<sf::Font>(m_Font));
		}

		inline const std::string& GetString() const { return m_String; }
		inline void SetString(const std::string& s)
		{ 
			m_String = s;
			m_TextObject.setString(m_String);
		}

		inline int GetFontSize() const { return m_FontSize; }
		inline void SetFontSize(int newFontSize) 
		{ 
			m_FontSize = newFontSize;
			m_TextObject.setCharacterSize(m_FontSize);
		}

		inline const sf::Color& GetColor() const { return m_TextColor; }
		inline void SetColor(const sf::Color& color) 
		{ 
			m_TextColor = color;
			m_TextObject.setFillColor(m_TextColor);
		}

		
		inline const sf::Vector2f& GetPadding() const { return m_Padding; }
		inline void SetPadding(const sf::Vector2f& padding) { m_Padding = padding; }


		inline virtual sf::Vector2f GetSize() const override
		{
			auto bounds = m_TextObject.getLocalBounds();
			return { bounds.width + m_Padding.x, bounds.height + m_Padding.y };
		}
		inline virtual void SetScale(const sf::Vector2f& scale) override
		{
			m_TextObject.setScale(scale);
		}


		inline const sf::Text& GetTextObject() const { return m_TextObject; }



	private:
		bool m_Modified = false;

		ResourceID m_Font;
		std::string m_String;
		int m_FontSize;
		sf::Color m_TextColor;

		sf::Vector2f m_Padding;

		sf::Text m_TextObject;
	};

}
