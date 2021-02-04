#include "Renderer.h"

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include "../Core.h"

namespace SFMLEngine {

	sf::ContextSettings* Renderer::s_ContextSettings = nullptr;

	const sf::ContextSettings& Renderer::Init()
	{
		// create the context settings
		s_ContextSettings = new sf::ContextSettings();
		s_ContextSettings->depthBits = 24;
		s_ContextSettings->majorVersion = 4;
		s_ContextSettings->minorVersion = 6;

		// Output the context settings and shader capabilities of the hardware
		LOG_CORE_INFO("OpenGL Version: {0}.{1}", s_ContextSettings->majorVersion, s_ContextSettings->minorVersion);
		LOG_CORE_INFO("Shaders Supported: {0}", (sf::Shader::isAvailable() ? "Yes" : "No"));

		return *s_ContextSettings;
	}

	void Renderer::InitGLEW()
	{
		// initialise glew
		// this must be after a valid openGL context is created
		GLenum err = glewInit();
		SFMLE_CORE_ASSERT(err == GLEW_OK, "GLEW failed to initialise.");


		// set up the opengl states we want
		glDepthFunc(GL_LEQUAL);
	}

	void Renderer::Shutdown()
	{
		delete s_ContextSettings;
	}

	void Renderer::Clear()
	{
		// SFML only clears the colour buffer,
		// but since were also using the depth buffer we need to clear that too
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::SetOpenGLStates()
	{
		glEnable(GL_DEPTH_TEST);
	}

}
