#include "DebugTools.h"

#include "Material.h"


namespace SFMLEngine {


	DebugTools::DebugTools(sf::RenderWindow* window)
		: m_Geometry(sf::Triangles)
	{
		m_WindowHandle = window;

		m_DebugRectTexture = ResourceManager::LoadFromFile<sf::Texture>("assets/textures/debugBox.png");
		m_DebugMaterial = Material::Create("Debug");
	}

	DebugTools::~DebugTools()
	{
		ResourceManager::DeleteResource<sf::Texture>(m_DebugRectTexture);
	}

	void DebugTools::Clear()
	{
		m_Geometry.clear();
		m_TriangleIndex = 0;
	}

	void DebugTools::DrawRect(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color)
	{
		ZoneScoped;

		m_Geometry.resize(m_Geometry.getVertexCount() + 6);


		m_Geometry[m_TriangleIndex + 0] = sf::Vertex(pos, color, sf::Vector2f(0, 0));
		m_Geometry[m_TriangleIndex + 1] = sf::Vertex(pos + sf::Vector2f(size.x, 0), color, sf::Vector2f(32, 0));
		m_Geometry[m_TriangleIndex + 2] = sf::Vertex(pos + size, color, sf::Vector2f(32, 32));

		m_Geometry[m_TriangleIndex + 3] = sf::Vertex(pos, color, sf::Vector2f(0, 0));
		m_Geometry[m_TriangleIndex + 4] = sf::Vertex(pos + sf::Vector2f(0, size.y), color, sf::Vector2f(0, 32));
		m_Geometry[m_TriangleIndex + 5] = sf::Vertex(pos + size, color, sf::Vector2f(32, 32));

		m_TriangleIndex += 6;
	}

	void DebugTools::DrawAll()
	{
		ZoneScoped;
		// draw the debug geometry using the debug material

		sf::Shader* shader = ResourceManager::GetResourceHandle<Material>(m_DebugMaterial)->GetShaderPtr();
		sf::Texture* texture = ResourceManager::GetResourceHandle<sf::Texture>(m_DebugRectTexture);
		
		sf::RenderStates renderState;
		renderState.shader = shader;
		renderState.texture = texture;

		m_WindowHandle->draw(m_Geometry, renderState);
		
		Clear();
	}

}
