#include "Application.h"

#include "ECS/Components.h"

#include <iostream>

namespace SFMLEngine
{

    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        if (!s_Instance)
            s_Instance = this;

        m_Window = new sf::RenderWindow(sf::VideoMode(800, 600), "SFML window");

        /*
        ----------
        SET UP ECS
        ----------
        */

        m_Coordinator = new Coordinator();
        m_Coordinator->Init();

        // register components
        m_Coordinator->RegisterComponent<Transform>();
        m_Coordinator->RegisterComponent<SpriteRenderer>();
        m_Coordinator->RegisterComponent<NativeScripts>();

        // register systems

        // render system
        m_RenderSystem = m_Coordinator->RegisterSystem<RenderSystem>();
        m_RenderSystem->Init(m_Coordinator, m_Window);

        {
            Signature signature;
            signature.set(m_Coordinator->GetComponentType<Transform>());
            signature.set(m_Coordinator->GetComponentType<SpriteRenderer>());
            m_Coordinator->SetSystemSignature<RenderSystem>(signature);
        }

        // native scripting system
        m_ScriptableEntitySystem = m_Coordinator->RegisterSystem<ScriptableEntitySystem>();
        m_ScriptableEntitySystem->Init(m_Coordinator);

        {
            Signature signature;
            signature.set(m_Coordinator->GetComponentType<NativeScripts>());
            m_Coordinator->SetSystemSignature<ScriptableEntitySystem>(signature);
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
            

            // Update the systems


            // update the render system last
            m_RenderSystem->Update(0);



            // Clear screen
            m_Window->clear();

            // draw to the screen
            m_RenderSystem->Render();

            // Update the window
            m_Window->display();
        }

        Shutdown();
    }

    void Application::Shutdown()
    {
        size_t numLayers = m_LayerStack.GetNumLayers();
        for (size_t i = 0; i < numLayers; i++)
        {
            Layer* currentLayer = m_LayerStack.GetLayer(0);
            m_LayerStack.PopLayer(currentLayer);
        }
    }


}
