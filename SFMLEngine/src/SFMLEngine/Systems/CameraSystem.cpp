#include "CameraSystem.h"

#include "SFMLEngine/ECS/Components/Transform.h"
#include "SFMLEngine/ECS/Components/Camera.h"

namespace SFMLEngine {

	void CameraSystem::Init(Coordinator* coordinator)
	{
		m_Coordinator = coordinator;
	}

	void CameraSystem::EntityAddedToSystem(Entity entity)
	{
		Camera& camera = m_Coordinator->GetComponent<Camera>(entity);
		if (camera.IsMain()) m_MainCamera = entity;
	}

	void CameraSystem::Update()
	{
		ZoneScoped;
		for (const auto& entity : m_Entities)
		{
			auto& cam = m_Coordinator->GetComponent<Camera>(entity);
			if (ComponentModified(cam))
			{
				ResetModified(cam);
				if (cam.IsMain())
				{
					m_MainCamera = entity;
					break;
				}
			}
		}
	}

	void CameraSystem::WindowResized(const sf::Vector2f& newSize)
	{
		ZoneScoped;
		for (auto const& e : m_Entities)
		{
			auto& cam = m_Coordinator->GetComponent<Camera>(e);
			cam.SetSize(newSize);
		}
	}

	sf::View CameraSystem::GetMainCameraView()
	{
		ZoneScoped;
		auto& cam = m_Coordinator->GetComponent<Camera>(m_MainCamera);
		auto& transform = m_Coordinator->GetComponent<Transform>(m_MainCamera);

		sf::View view(transform.Position, cam.GetSize());
		view.setViewport(cam.GetViewport());

		view.setRotation(transform.Rotation);
		view.zoom(cam.GetZoom());

		return view;
	}

	bool CameraSystem::ValidMainCamera()
	{
		return m_MainCamera != INVALID_ENTITY_ID;
	}

}