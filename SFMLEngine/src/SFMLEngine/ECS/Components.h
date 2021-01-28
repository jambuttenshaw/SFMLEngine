#pragma once

#include <SFML/Graphics.hpp>

#include "ScriptableEntity.h"
#include "../ResourceManagement/ResourceManager.h"


namespace SFMLEngine {

	struct Transform 
	{
		sf::Vector2f Position;
		float Rotation = 0.0f;
		sf::Vector2f Scale;

		Transform()
			: Position(sf::Vector2f(0, 0)), Rotation(0.0f), Scale(sf::Vector2f(1.0f, 1.0f))
		{}
		Transform(sf::Vector2f pos, float rot, sf::Vector2f scale)
			: Position(pos), Rotation(rot), Scale(scale)
		{}
	};

	struct SpriteRenderer
	{
		ResourceID TextureHandle;
		int OrderInLayer;
		sf::Sprite Sprite;

		SpriteRenderer()
			: TextureHandle(NULL_RESOURCE_ID), OrderInLayer(0), Sprite()
		{}
		SpriteRenderer(ResourceID texHandle, int orderInLayer)
			: TextureHandle(texHandle), OrderInLayer(orderInLayer), Sprite()
		{}
	};

	struct NativeScripts
	{
		std::unordered_map<const char*, ScriptableEntity*> Scripts;
	};

}
