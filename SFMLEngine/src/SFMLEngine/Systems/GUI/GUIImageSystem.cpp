#include "GUIImageSystem.h"


#include "SFMLEngine/ECS/Components/GUI/GUIImage.h"
#include "SFMLEngine/ECS/Components/GUI/GUITransform.h"

namespace SFMLEngine {

	void GUIImageSystem::Init(Coordinator* coordinator, sf::RenderWindow* window)
	{
		m_Coordinator = coordinator;
		m_RenderWindow = window;
	}

	void GUIImageSystem::EntityAddedToSystem(Entity entity)
	{
		auto& image = m_Coordinator->GetComponent<GUIImage>(entity);
		if (image.TextureHandle != NULL_RESOURCE_ID)
		{
			image.Sprite.setTexture(*ResourceManager::GetResourceHandle<sf::Texture>(image.TextureHandle));
		}
		else
		{
			LOG_CORE_WARN("No image was supplied for entity {0}", entity);
		}
	}

	void GUIImageSystem::EntityRemovedFromSystem(Entity entity)
	{
	}

	void GUIImageSystem::Render()
	{
		ZoneScoped;
		sf::RenderStates renderState;

		for (auto& e : m_Entities)
		{
			GUIImage& image = m_Coordinator->GetComponent<GUIImage>(e);
			GUITransform& transform = m_Coordinator->GetComponent<GUITransform>(e);

			renderState.transform = transform.GetTransformMatrix();
			m_RenderWindow->draw(image.Sprite, renderState);
		}
	}

}