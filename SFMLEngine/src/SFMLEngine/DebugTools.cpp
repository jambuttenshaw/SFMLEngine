#include "DebugTools.h"

#include "Renderer/Material.h"
#include "Renderer/Texture.h"


namespace SFMLEngine {

	sf::RenderWindow* DebugTools::s_WindowHandle = nullptr;

	ResourceID DebugTools::s_DebugRectTexture = NULL_RESOURCE_ID;
	ResourceID DebugTools::s_DebugMaterial = NULL_RESOURCE_ID;

	sf::VertexArray DebugTools::s_Geometry(sf::Triangles);
	size_t DebugTools::s_TriangleIndex = 0;

	void DebugTools::Init(sf::RenderWindow* window)
	{
		s_WindowHandle = window;

		s_DebugRectTexture = Texture::Create("assets/textures/debugBox.png");
		s_DebugMaterial = Material::Create("Debug");
	}

	void DebugTools::Shutdown()
	{
		Texture::Destroy(s_DebugRectTexture);
	}

	void DebugTools::Clear()
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

	void DebugTools::DrawAll()
	{
		ZoneScoped;
		// draw the debug geometry using the debug material

		sf::Shader* shader = ResourceManager::GetResourceHandle<Material>(s_DebugMaterial)->GetShaderPtr();
		sf::Texture* texture = ResourceManager::GetResourceHandle<sf::Texture>(s_DebugRectTexture);
		
		sf::RenderStates renderState;
		renderState.shader = shader;
		renderState.texture = texture;

		s_WindowHandle->draw(s_Geometry, renderState);
		
		Clear();
	}

}
