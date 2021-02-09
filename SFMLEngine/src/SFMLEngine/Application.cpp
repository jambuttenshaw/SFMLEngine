#include "Application.h"

#include "ECS/Components.h"
#include "Timestep.h"

#include "Input.h"

#include "Renderer/Renderer.h"
#include "Renderer/Material.h"
#include "Renderer/Texture.h"
#include "Renderer/TilePalette.h"

#include "FontLibrary.h"

#include <Tracy.hpp>

namespace SFMLEngine
{

    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name, const sf::Vector2i& windowDimensions)
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

        m_Window = new sf::RenderWindow(sf::VideoMode(windowDimensions.x, windowDimensions.y), name, sf::Style::Default, contextSettings);
        m_Window->setVerticalSyncEnabled(true);

        Renderer::InitGLEW();

        // load up the shader library
        ShaderLibrary::Init();
        // load up the font library
        FontLibrary::Init();

        
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
        m_Coordinator->RegisterComponent<DirectionalLight>();
        m_Coordinator->RegisterComponent<Text>();
        m_Coordinator->RegisterComponent<Camera>();
        m_Coordinator->RegisterComponent<Tilemap>();
        m_Coordinator->RegisterComponent<TilemapRenderer>();

        // register systems

        // sprite render system
        m_SpriteRenderSystem = m_Coordinator->RegisterSystem<SpriteRenderSystem>();
        m_SpriteRenderSystem->Init(m_Coordinator, m_Window);
        {
            Signature signature;
            signature.set(m_Coordinator->GetComponentType<Transform>());
            signature.set(m_Coordinator->GetComponentType<SpriteRenderer>());
            m_Coordinator->SetSystemSignature<SpriteRenderSystem>(signature);
        }

        // tilemap render system
        m_TilemapRenderSystem = m_Coordinator->RegisterSystem<TilemapRenderSystem>();
        m_TilemapRenderSystem->Init(m_Coordinator, m_Window);
        {
            Signature signature;
            signature.set(m_Coordinator->GetComponentType<Transform>());
            signature.set(m_Coordinator->GetComponentType<Tilemap>());
            signature.set(m_Coordinator->GetComponentType<TilemapRenderer>());
            m_Coordinator->SetSystemSignature<TilemapRenderSystem>(signature);
        }


        // native scripting system
        m_ScriptableEntitySystem = m_Coordinator->RegisterSystem<ScriptableEntitySystem>();
        m_ScriptableEntitySystem->Init(m_Coordinator);
        {
            Signature signature;
            signature.set(m_Coordinator->GetComponentType<NativeScripts>());
            m_Coordinator->SetSystemSignature<ScriptableEntitySystem>(signature);
        }

        // lighting systems
        m_PointLightSystem = m_Coordinator->RegisterSystem<PointLightSystem>();
        m_PointLightSystem->Init(m_Coordinator);
        {
            Signature signature;
            signature.set(m_Coordinator->GetComponentType<Transform>());
            signature.set(m_Coordinator->GetComponentType<PointLight>());
            m_Coordinator->SetSystemSignature<PointLightSystem>(signature);
        }
        m_DirectionalLightSystem = m_Coordinator->RegisterSystem<DirectionalLightSystem>();
        m_DirectionalLightSystem->Init(m_Coordinator);
        {
            Signature signature;
            signature.set(m_Coordinator->GetComponentType<DirectionalLight>());
            m_Coordinator->SetSystemSignature<DirectionalLightSystem>(signature);
        }


        // GUI system
        m_GUISystem = m_Coordinator->RegisterSystem<GUISystem>();
        m_GUISystem->Init(m_Coordinator, m_Window);
        {
            Signature signature;
            signature.set(m_Coordinator->GetComponentType<Transform>());
            signature.set(m_Coordinator->GetComponentType<Text>());
            m_Coordinator->SetSystemSignature<GUISystem>(signature);
        }


        // Camera system
        m_CameraSystem = m_Coordinator->RegisterSystem<CameraSystem>();
        m_CameraSystem->Init(m_Coordinator);
        {
            Signature signature;
            signature.set(m_Coordinator->GetComponentType<Transform>());
            signature.set(m_Coordinator->GetComponentType<Camera>());
            m_Coordinator->SetSystemSignature<CameraSystem>(signature);
        }

        /*
        ----------
        INIT INPUT
        ----------
        */

        Input::Init(m_Window, m_CameraSystem);


        // debug info
        m_DebugText.setFillColor(sf::Color::White);
        m_DebugText.setFont(*ResourceManager::GetResourceHandle<sf::Font>(FontLibrary::GetFont("arial")));
        m_DebugText.setCharacterSize(16);
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

        float fps = 0;

        while (m_Window->isOpen())
        {
            Timestep ts(m_Clock.restart().asSeconds());
            
            if (m_DisplayDebug)
            {
                fps = 1 / ts;
                m_DebugInfo.push_back("FPS: " + std::to_string(fps));
            }


            Input::ResetDeltas();

            // Process events
            sf::Event event;
            while (m_Window->pollEvent(event))
            {
                // window events should be handled by the application
                if (event.type == sf::Event::Closed)
                {
                    m_Window->close();
                    continue;
                }
                
                if (event.type == sf::Event::Resized)
                {
                    m_CameraSystem->WindowResized(sf::Vector2f((float)event.size.width, (float)event.size.height));
                    continue;
                }

                if (event.type == sf::Event::MouseMoved)
                    Input::SetMouseDelta(sf::Vector2f((float)event.mouseMove.x, (float)event.mouseMove.y));

                if (event.type == sf::Event::MouseWheelMoved)
                    Input::SetWheelDelta((float)event.mouseWheel.delta);

                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::F3)
                    {
                        m_DisplayDebug = !m_DisplayDebug;
                        continue;
                    }
                }
                
                // send event callback through to the layer stack
                for (Layer* layer : m_LayerStack)
                {
                    layer->OnEvent(event);
                }
            }
            
            // update the layers
            for (Layer* layer : m_LayerStack)
            {
                layer->Update(ts);
            }

            // Update the systems

            // upadte all native scripts
            m_ScriptableEntitySystem->Update(ts);

            // apply any changes made to components
            m_SpriteRenderSystem->Update();
            m_TilemapRenderSystem->Update();
            m_GUISystem->Update();
            m_CameraSystem->Update();

            // ---------
            // RENDERING
            // ---------
            
            // Clear screen
            m_Window->clear();
            Renderer::Clear();

            // set the context active
            m_Window->setActive();

            // set up the renderer for drawing
            Renderer::SetOpenGLStates();

            {
                // upload all lighting data to shaders
                m_PointLightSystem->UploadAllLightingData();
                m_DirectionalLightSystem->UploadAllLightingData();

                // set the windows view for the game world using the main camera
                m_Window->setView(m_CameraSystem->GetMainCameraView());

                // draw to the screen

                // since I have used the opengl depth buffer for ordering every drawable,
                // I dont need to care about the order in which I draw them
                m_SpriteRenderSystem->Render();
                m_TilemapRenderSystem->Render();
            }


            {
                // set the windows view to the default for the GUI
                m_Window->setView(m_Window->getDefaultView());
                // draw the GUI onto the display
                m_GUISystem->Render();
            }


            if (m_DisplayDebug) 
            {
                // collect debug info

                float y = 0;
                for (auto const& s : m_DebugInfo)
                {
                    // display debug info
                    m_DebugText.setPosition(0, y);
                    m_DebugText.setString(s);
                    m_Window->draw(m_DebugText);

                    y += 10;
                }
                m_DebugInfo.clear();
            }

            m_Window->setActive(false);

            // Update the window
            m_Window->display();

            FrameMark;
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

        TilePalette::DestroyAllCached();
        Material::DestroyAllCached();
        Texture::DestroyAllCached();

        FontLibrary::Shutdown();
        ShaderLibrary::Shutdown();
        ResourceManager::Shutdown();
    }


}
