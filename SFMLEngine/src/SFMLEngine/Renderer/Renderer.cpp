#include "Renderer.h"

# include <SFML/OpenGL.hpp>

#include "../Log.h"

namespace SFMLEngine {

	sf::ContextSettings* Renderer::s_ContextSettings = nullptr;

	const sf::ContextSettings& Renderer::Init()
	{
		// create the context settings
		s_ContextSettings = new sf::ContextSettings();
		s_ContextSettings->depthBits = 24;
		s_ContextSettings->majorVersion = 3;
		s_ContextSettings->minorVersion = 4;

		// Output the context settings and shader capabilities of the hardware
		LOG_CORE_INFO("OpenGL Version: {0}.{1}", s_ContextSettings->majorVersion, s_ContextSettings->minorVersion);
		LOG_CORE_INFO("Shaders Supported: {0}", (sf::Shader::isAvailable() ? "True" : "False"));
		LOG_CORE_INFO("Size allocated to depth buffer: {0} bits", s_ContextSettings->depthBits);

		return *s_ContextSettings;
	}

	void Renderer::Shutdown()
	{
		delete s_ContextSettings;
	}

	void Renderer::Begin()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::SetOpenGLStates()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}

}
