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

		ResourceID GetFont() { return m_Font; }
		void SetFont(ResourceID newFont)
		{ 
			m_Font = newFont;
			m_TextObject.setFont(*ResourceManager::GetResourceHandle<sf::Font>(m_Font));
		}

		const std::string& GetString() { return m_String; }
		void SetString(const std::string& s)
		{ 
			m_String = s;
			m_TextObject.setString(m_String);
		}

		int GetFontSize() { return m_FontSize; }
		void SetFontSize(int newFontSize) 
		{ 
			m_FontSize = newFontSize;
			m_TextObject.setCharacterSize(m_FontSize);
		}

		const sf::Color& GetColor() { return m_TextColor; }
		void SetColor(const sf::Color& color) 
		{ 
			m_TextColor = color;
			m_TextObject.setFillColor(m_TextColor);
		}

		
		const sf::Vector2f& GetPadding() { return m_Padding; }
		void SetPadding(const sf::Vector2f& padding) { m_Padding = padding; }


		virtual sf::Vector2f GetSize() override
		{
			auto bounds = m_TextObject.getLocalBounds();
			return { bounds.width + m_Padding.x, bounds.height + m_Padding.y };
		}
		virtual void SetScale(const sf::Vector2f& scale) override
		{
			m_TextObject.setScale(scale);
		}


		const sf::Text& GetTextObject() { return m_TextObject; }



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
