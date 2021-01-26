#pragma once

#include <SFML/Graphics.hpp>

#include "LayerStack.h"

namespace SFMLEngine {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		sf::RenderWindow* GetWindowHandle() { return m_Window; }

	private:
		sf::RenderWindow* m_Window;

		LayerStack m_LayerStack;


	public:
		static Application* GetApplicationHandle() { return s_Instance; }

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
