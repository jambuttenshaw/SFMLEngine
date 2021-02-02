#include "Application.h"

#include "ECS/Components.h"
#include "Timestep.h"

#include "ResourceManagement/ResourceManager.h"
#include "Renderer/Renderer.h"
#include "Renderer/ShaderLibrary.h"
#include "Renderer/Material.h"
#include "Renderer/Texture.h"

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
        ------------------------
        SET UP RENDERER & WINDOW
        ------------------------
        */

        auto const& contextSettings = Renderer::Init();

        m_Window = new sf::RenderWindow(sf::VideoMode(800, 600), "SFML window", sf::Style::Default, contextSettings);
        m_Window->setFramerateLimit(60);

        Renderer::InitGLEW();

        // load up the shader library
        ShaderLibrary::Init();

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
        m_Coordinator->RegisterComponent<PointLight>();

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


        // lighting system
        m_LightingSystem = m_Coordinator->RegisterSystem<LightingSystem>();
        m_LightingSystem->Init(m_Coordinator);

        {
            Signature signature;
            signature.set(m_Coordinator->GetComponentType<PointLight>());
            m_Coordinator->SetSystemSignature<LightingSystem>(signature);
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

        // shutdown the renderer
        Renderer::Shutdown();

        Material::DestroyAllCached();
        Texture::DestroyAllCached();

        ShaderLibrary::Shutdown();
        ResourceManager::Shutdown();
    }


}
