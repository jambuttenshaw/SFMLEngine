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
			: Position(sf::Vector2f(0, 0)), Rotation(0), Scale(sf::Vector2f(1, 1))
		{}
		Transform(const sf::Vector2f& pos)
			: Position(pos), Rotation(0), Scale(sf::Vector2f(1, 1))
		{}
		Transform(const sf::Vector2f& pos, float rot, const sf::Vector2f& scale)
			: Position(pos), Rotation(rot), Scale(scale)
		{}

		sf::Transform GetTransformMatrix() const 
		{
			sf::Transform t;
			t.translate(Position);
			t.rotate(Rotation);
			t.scale(Scale);
			return t;
		}
	};

	struct SpriteRenderer
	{
		ResourceID TextureHandle;
		ResourceID MaterialHandle;
		int RenderLayer;
		int OrderInLayer;
		ResourceID NormalMapHandle;
		sf::Sprite Sprite;

		SpriteRenderer()
			: TextureHandle(NULL_RESOURCE_ID), MaterialHandle(NULL_RESOURCE_ID), RenderLayer(0), OrderInLayer(0), NormalMapHandle(NULL_RESOURCE_ID), Sprite()
		{}
		SpriteRenderer(ResourceID texHandle, ResourceID material, int renderLayer, int orderInLayer)
			: TextureHandle(texHandle), MaterialHandle(material), RenderLayer(renderLayer), OrderInLayer(orderInLayer), NormalMapHandle(NULL_RESOURCE_ID), Sprite()
		{}
		SpriteRenderer(ResourceID texHandle, ResourceID material, int renderLayer, int orderInLayer, ResourceID normalMap)
			: TextureHandle(texHandle), MaterialHandle(material), RenderLayer(renderLayer), OrderInLayer(orderInLayer), NormalMapHandle(normalMap), Sprite()
		{}

		sf::Texture* GetTexture() const
		{
			return ResourceManager::GetResourceHandle<sf::Texture>(TextureHandle);
		}

		Material* GetMaterial() const
		{
			return ResourceManager::GetResourceHandle<Material>(MaterialHandle);
		}

		sf::Texture* GetNormalMap() const
		{
			return ResourceManager::GetResourceHandle<sf::Texture>(NormalMapHandle);
		}
	};

	struct NativeScripts
	{
		std::unordered_map<const char*, ScriptableEntity*> Scripts;
	};

	struct PointLight
	{
		float Intensity;
		float Range;
		sf::Color Color;
		bool Static;

		PointLight()
			: Range(10.0f), Intensity(1.0f), Color(sf::Color::White), Static(false)
		{}
		PointLight(float intensity, float range, const sf::Color& color, bool staticLight = false)
			: Intensity(intensity), Range(range), Color(color), Static(staticLight)
		{}
	};

	struct DirectionalLight
	{
		sf::Vector3f Direction;
		float Intensity;
		sf::Color Color;
		bool Static;

		DirectionalLight()
			: Direction(sf::Vector3f(0, 0, 1)), Intensity(1), Color(sf::Color::White), Static(false)
		{}
		DirectionalLight(const sf::Vector3f& direction, float intensity, const sf::Color& color, bool staticLight = false)
			: Direction(direction), Intensity(intensity), Color(color), Static(staticLight)
		{}
	};

}
