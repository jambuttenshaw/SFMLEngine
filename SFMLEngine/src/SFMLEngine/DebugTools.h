#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ResourceManagement/ResourceManager.h"


namespace SFMLEngine {

	class DebugTools {
	public:
		static void Init(sf::RenderWindow* window);
		static void Shutdown();

		static void DrawRect(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color);
		static void DrawRect(const sf::FloatRect& rect, const sf::Color& color);

		static void DrawLine(const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& color);

		static void ClearGameView();
		static void DrawAllGameView();

		static void CoreDisplay(const std::string& text);
		static void CoreDisplay(const std::string& label, float value);
		static void CoreDisplay(const std::string& label, int value);
		static void CoreDisplay(const std::string& label, bool value);
		static void CoreDisplay(const std::string& label, const sf::Vector2f& vec);


		static void AppDisplay(const std::string& text);
		static void AppDisplay(const std::string& label, float value);
		static void AppDisplay(const std::string& label, int value);
		static void AppDisplay(const std::string& label, bool value);
		static void AppDisplay(const std::string& label, const sf::Vector2f& vec);


		static void ClearHUDView();
		static void DrawAllHUDView();

	public:

		static inline const sf::Color COLLIDER_COLOR{ 0, 255, 0, 255 };
		static inline const sf::Color TRIGGER_COLOR{ 255, 255, 0, 255 };

	private:
		static sf::RenderWindow* s_WindowHandle;

		static ResourceID s_DebugRectTexture;
		static ResourceID s_DebugMaterial;

		static sf::VertexArray s_Geometry;
		static size_t s_TriangleIndex;

		static sf::Vertex s_Line[2];
		static std::vector<std::pair<sf::Vertex, sf::Vertex>> s_LinesToDraw;

		static std::vector<std::string> s_CoreDebugInfo;
		static sf::Text s_CoreDebugText;

		static std::vector<std::string> s_AppDebugInfo;
		static sf::Text s_AppDebugText;


		// constants
		static inline const int CHARACTER_SIZE = 16;
		static inline const int PADDING = 5;

		static inline const sf::Color CORE_COLOR{ 255, 127, 0 };
		static inline const sf::Color APP_COLOR{ 0, 127, 255 };
	};

}


// debug macros
#ifdef SFMLENGINE_DEBUG
	#define DEBUG_DRAW_RECT(...)		::SFMLEngine::DebugTools::DrawRect(__VA_ARGS__)
	#define DEBUG_DRAW_LINE(...)		::SFMLEngine::DebugTools::DrawLine(__VA_ARGS__)

	#define DEBUG_CORE_DISPLAY(...)		::SFMLEngine::DebugTools::CoreDisplay(__VA_ARGS__)

	#define DEBUG_DISPLAY(...)			::SFMLEngine::DebugTools::AppDisplay(__VA_ARGS__)
#else
	#define DEBUG_DRAW_RECT(...)		
	
	#define DEBUG_CORE_DISPLAY(...)		

	#define DEBUG_DISPLAY(...)		
#endif
