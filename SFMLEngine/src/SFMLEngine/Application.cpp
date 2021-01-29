#include "Application.h"

#include "ECS/Components.h"
#include "Timestep.h"
#include "ResourceManagement/ResourceManager.h"

#include "Log.h"

namespace SFMLEngine
{

    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        if (!s_Instance)
            s_Instance = this;

        /*
        --------------------------
        SET UP RESOURCE MANAGEMENT
        --------------------------
        */

        ResourceManager::Init();


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
        m_Window = m_RenderSystem->Init(m_Coordinator);

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

        /*
        start up the scriptable entities
        */
        m_ScriptableEntitySystem->Start();

        while (m_Window->isOpen())
        {
            Timestep ts(m_Clock.restart().asSeconds());

            // Process events
            sf::Event event;
            while (m_Window->pollEvent(event))
            {
                // window events should be handled by the application
                if (event.type == sf::Event::Closed)
                    m_Window->close();
                else
                {
                    // send event callback through to the layer stack
                    for (Layer* layer : m_LayerStack)
                    {
                        layer->OnEvent(event);
                    }
                }
            }
            
            // update the layers
            for (Layer* layer : m_LayerStack)
            {
                layer->Update(ts);
            }

            // Update the systems
            m_ScriptableEntitySystem->Update(ts);

            // update the render system last
            // the render system doesnt require a time step
            // as it is just applying any changes to the transforms
            // to the sprites
            m_RenderSystem->Update();



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
