#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "../ECS/Coordinator.h"
#include "../ECS/Components.h"

#include "../ResourceManagement/ResourceManager.h"

#include <algorithm>

namespace SFMLEngine {

	class RenderSystem : public System
	{
	public:
		RenderSystem() = default;
		~RenderSystem()
		{
			delete m_ContextSettings;
		}
		
		sf::RenderWindow* Init(Coordinator* coordinator)
		{
			m_Coordinator = coordinator;
			
			// create the context settings
			m_ContextSettings = new sf::ContextSettings();
			m_ContextSettings->depthBits = 24;
			m_ContextSettings->majorVersion = 3;
			m_ContextSettings->minorVersion = 4;
			

			m_RenderWindow = new sf::RenderWindow(sf::VideoMode(800, 600), "SFML window", sf::Style::Default, *m_ContextSettings);
			m_RenderWindow->setFramerateLimit(60);


			// Output the context settings and shader capabilities of the hardware
			LOG_CORE_INFO("OpenGL Version: {0}.{1}", m_ContextSettings->majorVersion, m_ContextSettings->minorVersion);
			LOG_CORE_INFO("Shaders Supported: {0}", (sf::Shader::isAvailable() ? "True" : "False"));
			LOG_CORE_INFO("Size allocated to depth buffer: {0} bits", m_ContextSettings->depthBits);

			
			// set up OpenGL
			m_RenderWindow->setActive();

			glDepthFunc(GL_LEQUAL);

			m_RenderWindow->setActive(false);
			

			// load the shader
			m_Shader = ResourceManager::LoadFromFile<sf::Shader>("assets/shaders/Basic.vert", "assets/shaders/Basic.frag");

			return m_RenderWindow;
		}

		sf::ContextSettings& GetContextSettings()
		{
			return *m_ContextSettings;
		}

		void SetWindowHandle(sf::RenderWindow* window) { m_RenderWindow = window; }

		void EntityAddedToSystem(Entity entity) override
		{
			auto& sRenderer = m_Coordinator->GetComponent<SpriteRenderer>(entity);
			if (sRenderer.TextureHandle != NULL_RESOURCE_ID)
			{
				sRenderer.Sprite.setTexture(*ResourceManager::GetResourceHandle<sf::Texture>(sRenderer.TextureHandle));
			}
			else
			{
				LOG_WARN("No texture was supplied for entity ID {0}", entity);
			}
			m_MaxOrderInLayer = std::max(m_MaxOrderInLayer, abs(sRenderer.OrderInLayer));
		}

		void Update()
		{
			for (auto const& entity : m_Entities)
			{
				auto& sRenderer = m_Coordinator->GetComponent<SpriteRenderer>(entity);
				auto& transform = m_Coordinator->GetComponent<Transform>(entity);

				sRenderer.Sprite.setPosition(transform.Position);
				sRenderer.Sprite.setRotation(transform.Rotation);
				sRenderer.Sprite.setScale(transform.Scale);
			}
		}

		void Render()
		{
			auto components = m_Coordinator->GetComponents<SpriteRenderer>(m_Entities);

			m_RenderWindow->setActive();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);

			auto shader = ResourceManager::GetResourceHandle<sf::Shader>(m_Shader);

			// we do not want to divide by 0
			float normalizeFactor = m_MaxOrderInLayer == 0 ? 1.0f : (float)m_MaxOrderInLayer;

			for (const auto& c : components)
			{
				// set shader uniforms
				shader->setUniform("u_DepthValue", (float)c.OrderInLayer / normalizeFactor);

				m_RenderWindow->draw(c.Sprite, shader);
			}

			m_RenderWindow->setActive(false);
		}

	private:
		Coordinator* m_Coordinator = nullptr;
		sf::RenderWindow* m_RenderWindow = nullptr;

		sf::ContextSettings* m_ContextSettings = nullptr;

		// the shader used to render sprites
		ResourceID m_Shader = NULL_RESOURCE_ID;

		int m_MaxOrderInLayer = 0;
	};

}
