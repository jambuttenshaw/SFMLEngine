#include "Application.h"

#include "ECS/Components.h"
#include "Timestep.h"

#include "Math.h"

#include "Input.h"

#include "Systems/Physics/Physics.h"

#include "Renderer/Renderer.h"
#include "Renderer/Material.h"
#include "Renderer/Texture.h"
#include "Renderer/TilePalette.h"

#include "FontLibrary.h"

#ifdef SFMLENGINE_DEBUG
    #include "DebugTools.h"
#endif


namespace SFMLEngine
{

    Application* Application::s_Instance = nullptr;


    Application::Application(const std::string& name, const sf::Vector2i& windowDimensions)
    {
        SFMLE_CORE_ASSERT(!s_Instance, "Application already exists!");
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
        
        m_Window->setFramerateLimit(m_FPSLimit);
        m_Window->setVerticalSyncEnabled(m_VSync);

        Renderer::InitGLEW();

        // load up the shader library
        ShaderLibrary::Init();
        // load up the font library
        FontLibrary::Init();

#ifdef SFMLENGINE_DEBUG
        // init debug tools
        DebugTools::Init(m_Window);
#endif
        
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

            m_Coordinator->RegisterComponent<Identity>();
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

            m_Coordinator->RegisterComponent<ColliderInfo>();
            m_Coordinator->RegisterComponent<BoxCollider>();
            m_Coordinator->RegisterComponent<CircleCollider>();
            m_Coordinator->RegisterComponent<TilemapCollider>();

            m_Coordinator->RegisterComponent<Animator>();
        }

        {
            ZoneScoped;
            ZoneName("InitSystems", 11);
            // register systems

            // identity system
            m_IdentitySystem = m_Coordinator->RegisterSystem<IdentitySystem>();
            m_IdentitySystem->Init(m_Coordinator);
            {
                Signature signature;
                signature.set(m_Coordinator->GetComponentType<Identity>());
                m_Coordinator->SetSystemSignature<IdentitySystem>(signature);
            }


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
                signature.set(m_Coordinator->GetComponentType<ColliderInfo>());
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


            m_AnimationSystem = m_Coordinator->RegisterSystem<AnimationSystem>();
            m_AnimationSystem->Init(m_Coordinator);
            {
                Signature signature;
                signature.set(m_Coordinator->GetComponentType<Animator>());
                signature.set(m_Coordinator->GetComponentType<SpriteRenderer>());
                m_Coordinator->SetSystemSignature<AnimationSystem>(signature);
            }
        }


        Renderer::SetRenderSystems(m_SpriteRenderSystem, m_TilemapRenderSystem);


        /*
        ----------
        INIT INPUT
        ----------
        */

        Input::Init(m_Window, m_CameraSystem);

        /*
        ------------
        INIT PHYSICS
        ------------
        */

        Physics::Init(m_CollisionSystem);

    }

    Application::~Application()
    {
        delete m_Window;
        delete m_Coordinator;
    }


    void Application::SetVSync(bool state)
    {
        m_VSync = state;
        m_Window->setVerticalSyncEnabled(state);
    }

    void Application::SetFPSLimit(unsigned int limit)
    {
        SFMLE_CORE_ASSERT(limit >= 0, "Invalid value for FPS limit.");

        m_FPSLimit = limit;
        m_Window->setFramerateLimit(limit);
    }

    void Application::Run()
    {

        // make sure the scene has a camera; otherwise rendering will not be possible
        SFMLE_CORE_ASSERT(m_CameraSystem->ValidMainCamera(), "No valid main camera in existence!");

        /*
        start up the scriptable entities
        */
        m_ScriptableEntitySystem->Start();

        m_Clock.restart();

        while (m_Window->isOpen())
        {
            Timestep ts(m_Clock.restart().asSeconds());

#ifdef SFMLENGINE_DEBUG
            if (m_DisplayDebug)
            {
                DEBUG_CORE_DISPLAY("FPS", 1 / ts);
            }
#endif

            {
                ZoneScoped;
                ZoneName("HandleInput", 11);

                Input::Reset();

                // Process events
                sf::Event event;
                while (m_Window->pollEvent(event))
                {
                    ZoneScoped;
                    ZoneName("HandleEvent", 11);
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
                        // send the key to input module to be registered as pressed this frame
                        Input::SetKeyPressed(event.key.code);

#ifdef SFMLENGINE_DEBUG
                        if (event.key.code == sf::Keyboard::F3)
                        {
                            m_DisplayDebug = !m_DisplayDebug;
                            continue;
                        }
#endif
                    }
                }
            }

            {
                ZoneScoped;
                ZoneName("Update", 6);

                // Update the systems

                // update all physics
                m_PhysicsSystem->Update(ts);


                // update all native scripts
                m_ScriptableEntitySystem->Update(ts);

                // animate sprites
                m_AnimationSystem->Update(ts);

                // apply any changes made to components
                m_IdentitySystem->Update();
                m_TilemapSystem->Update();
                m_GUISystem->Update();
                m_CameraSystem->Update();

#ifdef SFMLENGINE_DEBUG
                
                if (m_DisplayDebug)
                {
                    ZoneScoped;
                    ZoneName("GetDebugInfo", 12);

                    m_BoxColliderDebugSystem->Update();
                    
                    ResourceManager::DisplayDebug();
                    m_CurrentScene->DisplayDebug();
                }
#endif
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
                    ZoneName("Clear", 5);
                    m_Window->clear(m_ClearColor);
                }
            }

            {
                ZoneScoped;
                ZoneName("RenderWorld", 11);

                // upload all lighting data to shaders
                m_PointLightSystem->UploadAllLightingData();
                m_DirectionalLightSystem->UploadAllLightingData();

                // set the windows view for the game world using the main camera
                m_Window->setView(m_CameraSystem->GetMainCameraView());

                // draw to the screen
                Renderer::Render();
            }

#ifdef SFMLENGINE_DEBUG
            
            if (m_DisplayDebug)
            {
                ZoneScoped;
                ZoneName("RenderDebugGame", 15);

                // display debug game objects
                DebugTools::DrawAllGameView();
            }
            else DebugTools::ClearGameView();
#endif


            {
                ZoneScoped;
                ZoneName("RenderGUI", 9);

                // set the windows view to the default for the GUI
                m_Window->setView(m_Window->getDefaultView());
                // draw the GUI onto the display
                m_GUISystem->Render();
            }


#ifdef SFMLENGINE_DEBUG
            if (m_DisplayDebug) 
            {
                ZoneScoped;
                ZoneName("RenderDebugHUD", 14);

                // display debug info on the hud
                DebugTools::DrawAllHUDView();
            }
            else DebugTools::ClearHUDView();
#endif


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

#ifdef SFMLENGINE_DEBUG
        DebugTools::Shutdown();
#endif

        TilePalette::DestroyAllCached();
        Material::DestroyAllCached();
        Texture::DestroyAllCached();

        FontLibrary::Shutdown();
        ShaderLibrary::Shutdown();

        ResourceManager::Shutdown();

        Math::Shutdown();
    }


}
