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

        m_GUIView = m_Window->getDefaultView();

        Renderer::InitGLEW();

        // load up the shader library
        ShaderLibrary::Init();
        // load up the font library
        FontLibrary::Init();

#ifdef SFMLENGINE_DEBUG
        // init debug tools
        DebugTools::Init(m_Window);
#endif
        
        // this needs to be done before components are registered
        CollisionSystem::SetupColliderIDs();

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

            m_Coordinator->RegisterComponent<GUITransform>();
            m_Coordinator->RegisterComponent<GUIImage>();
            m_Coordinator->RegisterComponent<GUIText>();

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


            // GUI systems
            m_GUIPositionSystem = m_Coordinator->RegisterSystem<GUIPositionSystem>();
            m_GUIPositionSystem->Init(m_Coordinator, m_Window);
            {
                Signature signature;
                signature.set(m_Coordinator->GetComponentType<GUITransform>());
                m_Coordinator->SetSystemSignature<GUIPositionSystem>(signature);
            }

            m_GUIImageSystem = m_Coordinator->RegisterSystem<GUIImageSystem>();
            m_GUIImageSystem->Init(m_Coordinator, m_Window);
            {
                Signature signature;
                signature.set(m_Coordinator->GetComponentType<GUITransform>());
                signature.set(m_Coordinator->GetComponentType<GUIImage>());
                m_Coordinator->SetSystemSignature<GUIImageSystem>(signature);
            }
            m_GUITextSystem = m_Coordinator->RegisterSystem<GUITextSystem>();
            m_GUITextSystem->Init(m_Coordinator, m_Window);
            {
                Signature signature;
                signature.set(m_Coordinator->GetComponentType<GUITransform>());
                signature.set(m_Coordinator->GetComponentType<GUIText>());
                m_Coordinator->SetSystemSignature<GUITextSystem>(signature);
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
            m_TilemapSystem->Init(m_Coordinator, m_CollisionSystem);
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

        Physics::Init(m_Coordinator, m_CollisionSystem);
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

            //////////////////////
            // SCENE MANAGEMENT //
            //////////////////////


            // destroy entities that were queued to be deleted last frame
            for (Scene* scene : m_CurrentScenes)
            {
                scene->DestroyAllPending();
            }



            // check to see if there are any scenes that have been queued for deletion this frame
            if (m_ScenesToDelete.size())
            {
                for (Scene* scene : m_ScenesToDelete)
                {
                    // loop backwards through the current scenes
                    for (int index = static_cast<int>(m_CurrentScenes.size()) - 1; index >= 0; index--)
                    {
                        // if the scene at this index is to be deleted
                        if (m_CurrentScenes[index] == scene)
                        {
                            // delete the scene and remove it from the vector
                            scene->Destroy();
                            delete scene;
                            m_CurrentScenes.erase(m_CurrentScenes.begin() + index);
                        }
                    }
                }
                // remove all scenes that were flagged to be deleted
                m_ScenesToDelete.clear();
            }

            // check if we have to load in a new scene before beginning this frame
            // if there are multiple to load then we want to load them all
            if (m_ScenesToLoad.size())
            {
                // if any of the scenes to be loaded are single scenes, all existing scenes need to be removed
                int singleScenes = 0;
                for (auto& scene : m_ScenesToLoad)
                {
                    if (scene.second == LoadSceneMode::Single)
                    {
                        singleScenes++;
                    }
                }

                // there exists 1 single scene to be loaded, so we must delete all scenes currently loaded
                if (singleScenes == 1)
                {
                    DeleteAllCurrentScenes();
                }
                else if (singleScenes > 1)
                {
                    // error! we cannot load more than 1 single scenes in the same frame
                    SFMLE_CORE_ASSERT(0, "Cannot load more than 1 single scene in one frame.");
                }


                // we need to wait till all the new entities have been loaded
                // before calling start methods in scripts
                // to ensure that components the scripts need access to exist in the ecs first
                // so we stash all scripts and call their starts after loading

                
                if (m_CurrentScenes.size())
                {
                    // there are still some scenes existing in the game
                    // we do not want to call start on all of their entities,
                    // we only want to call start on the entities that have been added
                    m_ScriptableEntitySystem->SetSceneLoading(true);
                }
                else
                {
                    // if all scenes were removed,
                    // we can just consider the scriptable entity system as starting afresh
                    m_ScriptableEntitySystem->Reset();
                }


                // then load in the new ones
                // if there at multiple scenes at this point,
                // at least one of them must be additive
                for (auto& scene : m_ScenesToLoad)
                {
                    // to complete loading, the mode is now irrelevent
                    // either the existing scenes will be deleted to be replaced,
                    // or the still exist to be added to
                    
                    // so either way the only thing left to do is add in the new entities

                    CompleteLoadingNewScene(scene.first);
                }
                m_ScenesToLoad.clear();

                // if the scriptable entity system wasn't completely reset
                if (m_ScriptableEntitySystem->GetStarted())
                {
                    // we can just call start on the entities added in this scene loading process
                    m_ScriptableEntitySystem->SetSceneLoading(false);
                    m_ScriptableEntitySystem->StartAllPending();
                }
                else
                {
                    // otherwise call start on them all
                    m_ScriptableEntitySystem->Start();
                }


                // let entities know that a new scene has been loaded
                m_ScriptableEntitySystem->NewSceneLoaded();
            }

            ///////////
            // CLOCK //
            ///////////

            Timestep ts(m_Clock.restart().asSeconds());
            if (ts > m_DeltaTimeLimit) ts = m_DeltaTimeLimit;

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

                    else if (event.type == sf::Event::Resized)
                    {
                        m_CameraSystem->WindowResized(sf::Vector2f((float)event.size.width, (float)event.size.height));
                        m_GUIPositionSystem->RecalculateAll();

                        // set the view to fill the window with topleft at 0, 0
                        m_GUIView.reset({ 0, 0, static_cast<float>(event.size.width), static_cast<float>(event.size.height) });
                        continue;
                    }

                    else if (event.type == sf::Event::GainedFocus)
                    {
                        Input::SetFocused(true);
                        continue;
                    }

                    else if (event.type == sf::Event::LostFocus)
                    {
                        Input::SetFocused(false);
                        continue;
                    }

                    else if (event.type == sf::Event::MouseMoved)
                        Input::SetMouseDelta(sf::Vector2f((float)event.mouseMove.x, (float)event.mouseMove.y));

                    else if (event.type == sf::Event::MouseWheelMoved)
                        Input::SetWheelDelta(event.mouseWheel.delta);

                    else if (event.type == sf::Event::MouseButtonPressed)
                        Input::SetMousePressed(event.mouseButton.button);

                    else if (event.type == sf::Event::KeyPressed)
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

                    else if (event.type == sf::Event::KeyReleased)
                        Input::SetKeyUp(event.key.code);
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

                m_SpriteRenderSystem->Update();
                m_TilemapRenderSystem->Update();
                m_TilemapSystem->Update();

                m_GUIPositionSystem->Update();

                m_CameraSystem->Update();

#ifdef SFMLENGINE_DEBUG
                
                if (m_DisplayDebug)
                {
                    ZoneScoped;
                    ZoneName("GetDebugInfo", 12);

                    m_BoxColliderDebugSystem->Update();
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
                m_Window->setView(m_GUIView);

                // draw the GUI onto the display
                m_GUIImageSystem->Render();
                m_GUITextSystem->Render();
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
        for (Scene* scene : m_CurrentScenes)
        {
            scene->Destroy();
            delete scene;
        }
        m_CurrentScenes.clear();

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
