#include "Application.h"

#include "ECS/Components.h"


namespace SFMLEngine
{

    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        if (!s_Instance)
            s_Instance = this;

        m_Window = new sf::RenderWindow(sf::VideoMode(800, 600), "SFML window");


        m_Coordinator = new Coordinator();
        m_Coordinator->Init();

        m_Coordinator->RegisterComponent<Transform>();
        m_Coordinator->RegisterComponent<SpriteRenderer>();

        m_RenderSystem = m_Coordinator->RegisterSystem<RenderSystem>();
        m_RenderSystem->Init(m_Coordinator, m_Window);

        {
            Signature signature;
            signature.set(m_Coordinator->GetComponentType<SpriteRenderer>());
            m_Coordinator->SetSystemSignature<SpriteRenderer>(signature);
        }

    }

    Application::~Application()
    {
        delete m_Window;
        delete m_Coordinator;
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
                // window events should be handled by the application
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
