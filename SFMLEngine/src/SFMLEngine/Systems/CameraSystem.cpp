#include "CameraSystem.h"

#include "../ECS/Components/Transform.h"
#include "../ECS/Components/Camera.h"

namespace SFMLEngine {

	void CameraSystem::Init(Coordinator* coordinator)
	{
		m_Coordinator = coordinator;
	}

	void CameraSystem::EntityAddedToSystem(Entity entity)
	{
		Camera& camera = m_Coordinator->GetComponent<Camera>(entity);
		if (camera.Main) m_MainCamera = entity;
	}

	void CameraSystem::Update()
	{
		for (const auto& entity : m_Entities)
		{
			auto& cam = m_Coordinator->GetComponent<Camera>(entity);
			if (ComponentModified(cam))
			{
				ResetModified(cam);
				if (cam.Main)
				{
					m_MainCamera = entity;
					break;
				}
			}
		}
	}

	void CameraSystem::WindowResized(const sf::Vector2f& newSize)
	{
		for (auto const& e : m_Entities)
		{
			auto& cam = m_Coordinator->GetComponent<Camera>(e);
			cam.Size = newSize;
		}
	}

	sf::View CameraSystem::GetMainCameraView()
	{
		auto& cam = m_Coordinator->GetComponent<Camera>(m_MainCamera);
		auto& transform = m_Coordinator->GetComponent<Transform>(m_MainCamera);

		sf::View view(transform.Position, cam.Size);
		view.setViewport(cam.Viewport);

		view.setRotation(transform.Rotation);
		view.zoom(cam.Zoom < 0 ? 0 : cam.Zoom);

		return view;
	}

}