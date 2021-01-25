#include "Application.h"


namespace SFMLEngine
{

    Application::Application()
    {
        m_Window = new sf::RenderWindow(sf::VideoMode(800, 600), "SFML window");

    }

    Application::~Application()
    {
        delete m_Window;
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::Run()
    {
        while (m_Window->isOpen())
        {
            // Process events
            sf::Event event;
            while (m_Window->pollEvent(event))
            {
                // Close window: exit
                if (event.type == sf::Event::Closed)
                    m_Window->close();
                else
                {
                    for (Layer* layer : m_LayerStack)
                    {
                        layer->OnEvent(event);
                    }
                }
            }
            
            // Clear screen
            m_Window->clear();

            // Update the window
            m_Window->display();
        }
    }

}
