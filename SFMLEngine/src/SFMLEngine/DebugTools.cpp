#include "DebugTools.h"

#include "Renderer/Material.h"
#include "Renderer/Texture.h"

#include "FontLibrary.h"


namespace SFMLEngine {

	sf::RenderWindow* DebugTools::s_WindowHandle = nullptr;

	ResourceID DebugTools::s_DebugRectTexture = NULL_RESOURCE_ID;
	ResourceID DebugTools::s_DebugMaterial = NULL_RESOURCE_ID;

	sf::VertexArray DebugTools::s_Geometry(sf::Triangles);
	size_t DebugTools::s_TriangleIndex = 0;

	std::vector<std::string> DebugTools::s_CoreDebugInfo;
	sf::Text DebugTools::s_CoreDebugText;
	
	std::vector<std::string> DebugTools::s_AppDebugInfo;
	sf::Text DebugTools::s_AppDebugText;

	void DebugTools::Init(sf::RenderWindow* window)
	{
		s_WindowHandle = window;

		s_DebugRectTexture = Texture::Create("assets/textures/debugBox.png");
		s_DebugMaterial = Material::Create("Debug");


		s_CoreDebugText.setFillColor(CORE_COLOR);
		s_CoreDebugText.setFont(*ResourceManager::GetResourceHandle<sf::Font>(FontLibrary::GetFont("arial")));
		s_CoreDebugText.setCharacterSize(CHARACTER_SIZE);

		s_AppDebugText.setFillColor(APP_COLOR);
		s_AppDebugText.setFont(*ResourceManager::GetResourceHandle<sf::Font>(FontLibrary::GetFont("arial")));
		s_AppDebugText.setCharacterSize(CHARACTER_SIZE);
	}

	void DebugTools::Shutdown()
	{
		Texture::Destroy(s_DebugRectTexture);
	}

	void DebugTools::ClearGameView()
	{
		s_Geometry.clear();
		s_TriangleIndex = 0;
	}

	void DebugTools::DrawRect(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color)
	{
		ZoneScoped;

		s_Geometry.resize(s_Geometry.getVertexCount() + 6);


		s_Geometry[s_TriangleIndex + 0] = sf::Vertex(pos, color, sf::Vector2f(0, 0));
		s_Geometry[s_TriangleIndex + 1] = sf::Vertex(pos + sf::Vector2f(size.x, 0), color, sf::Vector2f(32, 0));
		s_Geometry[s_TriangleIndex + 2] = sf::Vertex(pos + size, color, sf::Vector2f(32, 32));

		s_Geometry[s_TriangleIndex + 3] = sf::Vertex(pos, color, sf::Vector2f(0, 0));
		s_Geometry[s_TriangleIndex + 4] = sf::Vertex(pos + sf::Vector2f(0, size.y), color, sf::Vector2f(0, 32));
		s_Geometry[s_TriangleIndex + 5] = sf::Vertex(pos + size, color, sf::Vector2f(32, 32));

		s_TriangleIndex += 6;
	}

	void DebugTools::DrawAllGameView()
	{
		ZoneScoped;
		// draw the debug geometry using the debug material

		sf::Shader* shader = ResourceManager::GetResourceHandle<Material>(s_DebugMaterial)->GetShaderPtr();
		sf::Texture* texture = ResourceManager::GetResourceHandle<sf::Texture>(s_DebugRectTexture);
		
		sf::RenderStates renderState;
		renderState.shader = shader;
		renderState.texture = texture;

		s_WindowHandle->draw(s_Geometry, renderState);
		
		ClearGameView();
	}



	void DebugTools::CoreDisplay(const std::string& text)
	{
		s_CoreDebugInfo.push_back(text);
	}
	void DebugTools::CoreDisplay(const std::string& label, float value)
	{
		s_CoreDebugInfo.push_back(label + ": " + std::to_string((int)round(value)));
	}
	void DebugTools::CoreDisplay(const std::string& label, int value)
	{
		s_CoreDebugInfo.push_back(label + ": " + std::to_string(value));
	}
	void DebugTools::CoreDisplay(const std::string& label, bool value)
	{
		s_CoreDebugInfo.push_back(label + ": " + std::to_string(value));
	}
	void DebugTools::CoreDisplay(const std::string& label, const sf::Vector2f& vec)
	{
		s_CoreDebugInfo.push_back(label + "  x: " + std::to_string((int)round(vec.x)) + " y: " + std::to_string((int)round(vec.y)));
	}


	void DebugTools::AppDisplay(const std::string& text)
	{
		s_AppDebugInfo.push_back(text);
	}
	void DebugTools::AppDisplay(const std::string& label, float value)
	{
		s_AppDebugInfo.push_back(label + ": " + std::to_string((int)round(value)));
	}
	void DebugTools::AppDisplay(const std::string& label, int value)
	{
		s_AppDebugInfo.push_back(label + ": " + std::to_string(value));
	}
	void DebugTools::AppDisplay(const std::string& label, bool value)
	{
		s_AppDebugInfo.push_back(label + ": " + std::to_string(value));
	}
	void DebugTools::AppDisplay(const std::string& label, const sf::Vector2f& vec)
	{
		s_AppDebugInfo.push_back(label + "  x: " + std::to_string((int)round(vec.x)) + " y: " + std::to_string((int)round(vec.y)));
	}



	void DebugTools::ClearHUDView()
	{
		s_CoreDebugInfo.clear();
		s_AppDebugInfo.clear();
	}

	void DebugTools::DrawAllHUDView()
	{
		float y = 0;
		for (auto const& s : s_CoreDebugInfo)
		{
			// display debug info
			s_CoreDebugText.setString(s);
			s_CoreDebugText.setPosition(s_WindowHandle->getSize().x - s_CoreDebugText.getLocalBounds().width - PADDING, y);
			s_WindowHandle->draw(s_CoreDebugText);

			y += CHARACTER_SIZE;
		}

		y = 0;
		for (auto const& s : s_AppDebugInfo)
		{
			// display debug info
			s_AppDebugText.setString(s);
			s_AppDebugText.setPosition(PADDING, y);
			s_WindowHandle->draw(s_AppDebugText);

			y += CHARACTER_SIZE;
		}

		ClearHUDView();
	}

}
