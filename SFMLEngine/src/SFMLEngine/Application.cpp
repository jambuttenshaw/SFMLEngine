#include "Application.h"

#include "ECS/Components.h"
#include "Timestep.h"

#include "Math.h"

#include "Input.h"

#include "Renderer/Renderer.h"
#include "Renderer/Material.h"
#include "Renderer/Texture.h"
#include "Renderer/TilePalette.h"

#include "FontLibrary.h"

#include "DebugTools.h"


namespace SFMLEngine
{

    Application* Application::s_Instance = nullptr;


    Application::Application(const std::string& name, const sf::Vector2i& windowDimensions)
    {
        if (!s_Instance)
            s_Instance = this;


        // ---------
        // INIT MATH
        // ---------

        Math::Init();

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


        // init debug tools
        DebugTools::Init(m_Window);

        
        /*
        ----------
        SET UP ECS
        ----------
        */
        {
            ZoneScoped;
            ZoneName("InitCoordinator", 15);

            m_Coordinator = new Coordinator();
            m_Coordinator->Init();
        }

        // register components
        {
            ZoneScoped;
            ZoneName("InitComponents", 14);
            m_Coordinator->RegisterComponent<Transform>();

            m_Coordinator->RegisterComponent<SpriteRenderer>();

            m_Coordinator->RegisterComponent<NativeScripts>();

            m_Coordinator->RegisterComponent<PointLight>();
            m_Coordinator->RegisterComponent<DirectionalLight>();

            m_Coordinator->RegisterComponent<Text>();

            m_Coordinator->RegisterComponent<Camera>();

            m_Coordinator->RegisterComponent<Tilemap>();
            m_Coordinator->RegisterComponent<TilemapRenderer>();

            m_Coordinator->RegisterComponent<Rigidbody>();

            m_Coordinator->RegisterComponent<Collider>();
            m_Coordinator->RegisterComponent<BoxCollider>();
            m_Coordinator->RegisterComponent<CircleCollider>();
            m_Coordinator->RegisterComponent<TilemapCollider>();
        }

        {
            ZoneScoped;
            ZoneName("InitSystems", 11);
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


            // collision system
            m_CollisionSystem = m_Coordinator->RegisterSystem<CollisionSystem>();
            m_CollisionSystem->Init(m_Coordinator);
            {
                Signature signature;
                signature.set(m_Coordinator->GetComponentType<Transform>());
                signature.set(m_Coordinator->GetComponentType<Collider>());
                m_Coordinator->SetSystemSignature<CollisionSystem>(signature);
            }


            // Physics system
            m_PhysicsSystem = m_Coordinator->RegisterSystem<PhysicsSystem>();
            m_PhysicsSystem->Init(m_Coordinator, m_CollisionSystem);
            {
                Signature signature;
                signature.set(m_Coordinator->GetComponentType<Transform>());
                signature.set(m_Coordinator->GetComponentType<Rigidbody>());
                m_Coordinator->SetSystemSignature<PhysicsSystem>(signature);
            }


            // Tilemap system
            m_TilemapSystem = m_Coordinator->RegisterSystem<TilemapSystem>();
            m_TilemapSystem->Init(m_Coordinator);
            {
                Signature signature;
                signature.set(m_Coordinator->GetComponentType<Transform>());
                signature.set(m_Coordinator->GetComponentType<Tilemap>());
                signature.set(m_Coordinator->GetComponentType<TilemapCollider>());
                m_Coordinator->SetSystemSignature<TilemapSystem>(signature);
            }

            // Box collider debugging system
            m_BoxColliderDebugSystem = m_Coordinator->RegisterSystem<BoxColliderDebugSystem>();
            m_BoxColliderDebugSystem->Init(m_Coordinator);
            {
                Signature signature;
                signature.set(m_Coordinator->GetComponentType<Transform>());
                signature.set(m_Coordinator->GetComponentType<BoxCollider>());
                m_Coordinator->SetSystemSignature<BoxColliderDebugSystem>(signature);
            }
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

    void Application::Run()
    {

        /*
        start up the scriptable entities
        */
        m_ScriptableEntitySystem->Start();

        int fps = 0;

        while (m_Window->isOpen())
        {
            Timestep ts(m_Clock.restart().asSeconds());

            if (m_DisplayDebug)
            {
                fps = (int)round(1 / ts);
                m_DebugInfo.push_back("FPS: " + std::to_string(fps));
            }

            {
                ZoneScoped;
                ZoneName("HandleInput", 11);

                Input::Reset();

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

                    if (event.type == sf::Event::GainedFocus)
                    {
                        Input::SetFocused(true);
                        continue;
                    }

                    if (event.type == sf::Event::LostFocus)
                    {
                        Input::SetFocused(false);
                        continue;
                    }

                    if (event.type == sf::Event::MouseMoved)
                        Input::SetMouseDelta(sf::Vector2f((float)event.mouseMove.x, (float)event.mouseMove.y));

                    if (event.type == sf::Event::MouseWheelMoved)
                        Input::SetWheelDelta((float)event.mouseWheel.delta);

                    if (event.type == sf::Event::MouseButtonPressed)
                        Input::SetMousePressed(event.mouseButton.button);

                    if (event.type == sf::Event::KeyPressed)
                    {
                        if (event.key.code == sf::Keyboard::F3)
                        {
                            m_DisplayDebug = !m_DisplayDebug;
                            continue;
                        }
                        
                        // send the key to input module to be registered as pressed this frame
                        Input::SetKeyPressed(event.key.code);
                    }
                }
            }

            {
                ZoneScoped;
                ZoneName("Update", 6);

                // Update the systems

                // update all native scripts
                m_ScriptableEntitySystem->Update(ts);


                // update all physics
                m_PhysicsSystem->Update(ts);


                // apply any changes made to components
                m_TilemapSystem->Update();
                if (m_DisplayDebug) m_BoxColliderDebugSystem->Update();
                m_SpriteRenderSystem->Update();
                m_TilemapRenderSystem->Update();
                m_GUISystem->Update();
                m_CameraSystem->Update();
            }

            // ---------
            // RENDERING
            // ---------
            {
                ZoneScoped;
                ZoneName("RenderBegin", 11);

                // Clear screen
                {
                    ZoneScoped;
                    ZoneName("WindowClear", 11);
                    m_Window->clear(sf::Color(0, 0, 0, 255));
                }
                Renderer::Clear();

                // set up the renderer for drawing
                Renderer::SetOpenGLStates();
            }

            {
                ZoneScoped;
                ZoneName("RenderWorld", 11);

                // upload all lighting data to shaders
                m_PointLightSystem->UploadAllLightingData();
                m_DirectionalLightSystem->UploadAllLightingData();

                // set the windows view for the game world using the main camera
                m_Window->setView(m_CameraSystem->GetMainCameraView());

                // set the material uniforms
                Renderer::SetUniforms();

                // draw to the screen

                // since I have used the opengl depth buffer for ordering every drawable,
                // I dont need to care about the order in which I draw them
                m_TilemapRenderSystem->Render();
                m_SpriteRenderSystem->Render();
            }

            if (m_DisplayDebug)
            {
                // display debug game objects
                DebugTools::DrawAll();
            }
            else DebugTools::Clear();


            {
                ZoneScoped;
                ZoneName("RenderGUI", 9);

                // set the windows view to the default for the GUI
                m_Window->setView(m_Window->getDefaultView());
                // draw the GUI onto the display
                m_GUISystem->Render();
            }


            if (m_DisplayDebug) 
            {
                ZoneScoped;
                ZoneName("RenderDebug", 11);
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

            // Update the window
            {
                ZoneScoped;
                ZoneName("Display", 7);

                m_Window->display();
            }

            FrameMark;
        }

        Shutdown();
    }

    void Application::Shutdown()
    {
        // delete the currently open scene
        if (m_CurrentScene)
        {
            m_CurrentScene->Destroy();
            delete m_CurrentScene;
        }

        // shutdown the renderer
        Renderer::Shutdown();

        DebugTools::Shutdown();

        TilePalette::DestroyAllCached();
        Material::DestroyAllCached();
        Texture::DestroyAllCached();

        FontLibrary::Shutdown();
        ShaderLibrary::Shutdown();

        ResourceManager::Shutdown();

        Math::Shutdown();
    }


}
