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

	private:
		sf::RenderWindow* m_Window;

		LayerStack m_LayerStack;

	};

	Application* CreateApplication();
}
