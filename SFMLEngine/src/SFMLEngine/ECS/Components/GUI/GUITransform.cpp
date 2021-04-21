#include "GUITransform.h"

#include "SFMLEngine/Application.h"


namespace SFMLEngine {


	sf::Vector2f GUITransform::GetTopLeft(const sf::Vector2f& elemSize) const
	{
		// topleft is easiest: just normalize the screen coordinates
		sf::Vector2f screenSize = sf::Vector2f(Application::GetWindowSize());
		return { m_ScreenPosition.x / screenSize.x, m_ScreenPosition.y / screenSize.y };
	}

	sf::Vector2f GUITransform::GetTopRight(const sf::Vector2f& elemSize) const
	{
		sf::Vector2f screenSize = sf::Vector2f(Application::GetWindowSize());
		return { (m_ScreenPosition.x + elemSize.x) / screenSize.x, m_ScreenPosition.y / screenSize.y };

	}

	sf::Vector2f GUITransform::GetBottomLeft(const sf::Vector2f& elemSize) const
	{
		sf::Vector2f screenSize = sf::Vector2f(Application::GetWindowSize());
		return { m_ScreenPosition.x / screenSize.x, (m_ScreenPosition.y + elemSize.y) / screenSize.y };

	}

	sf::Vector2f GUITransform::GetBottomRight(const sf::Vector2f& elemSize) const
	{
		sf::Vector2f screenSize = sf::Vector2f(Application::GetWindowSize());
		return { (m_ScreenPosition.x + elemSize.x) / screenSize.x, (m_ScreenPosition.y + elemSize.y) / screenSize.y };

	}


	void GUITransform::SetHorizontalAnchor(Anchor a)
	{
		if (a == Anchor::Top || a == Anchor::Bottom)
		{
			LOG_CORE_WARN("Cannot set horizontal anchor to Top or Bottom!");
			return;
		}
		m_HorizontalAnchor = a;

		m_Modified = true;
	}


	void GUITransform::SetVerticalAnchor(Anchor a)
	{
		if (a == Anchor::Left || a == Anchor::Right)
		{
			LOG_CORE_WARN("Cannot set Vertical anchor to Left or Right!");
			return;
		}
		m_VerticalAnchor = a;

		m_Modified = true;
	}
}