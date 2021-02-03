#pragma once

#include <SFML/Graphics.hpp>

#include "ScriptableEntity.h"
#include "../ResourceManagement/ResourceManager.h"

#include "../Renderer/Material.h"


namespace SFMLEngine {

	struct Transform 
	{
		sf::Vector2f Position;
		float Rotation = 0.0f;
		sf::Vector2f Scale;

		Transform()
			: Position(sf::Vector2f(0, 0)), Rotation(0.0f), Scale(sf::Vector2f(1.0f, 1.0f))
		{}
		Transform(sf::Vector2f pos)
			: Position(pos), Rotation(0.0f), Scale(sf::Vector2f(1.0f, 1.0f))
		{}
		Transform(sf::Vector2f pos, float rot, sf::Vector2f scale)
			: Position(pos), Rotation(rot), Scale(scale)
		{}
	};

	struct SpriteRenderer
	{
		ResourceID TextureHandle;
		ResourceID MaterialHandle;
		int RenderLayer;
		int OrderInLayer;
		sf::Sprite Sprite;

		SpriteRenderer()
			: TextureHandle(NULL_RESOURCE_ID), MaterialHandle(NULL_RESOURCE_ID), RenderLayer(0), OrderInLayer(0), Sprite()
		{}
		SpriteRenderer(ResourceID texHandle, ResourceID material, int renderLayer, int orderInLayer)
			: TextureHandle(texHandle), MaterialHandle(material), RenderLayer(renderLayer), OrderInLayer(orderInLayer), Sprite()
		{}

		sf::Texture* GetTexture() const
		{
			return ResourceManager::GetResourceHandle<sf::Texture>(TextureHandle);
		}

		Material* GetMaterial() const
		{
			return ResourceManager::GetResourceHandle<Material>(MaterialHandle);
		}
	};

	struct NativeScripts
	{
		std::unordered_map<const char*, ScriptableEntity*> Scripts;
	};

	struct PointLight
	{
		float Range;
		float Intensity;
		sf::Color Color;

		PointLight()
			: Range(10.0f), Intensity(1.0f), Color(sf::Color::White)
		{}
		PointLight(float range, float intensity, sf::Color color)
			: Range(range), Intensity(intensity), Color(color)
		{}
	};

}
