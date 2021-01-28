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
	};

	struct SpriteRenderer
	{
		sf::Sprite Sprite;
		ResourceID TextureHandle;
		int OrderInLayer;
	};

	struct NativeScripts
	{
		std::unordered_map<const char*, ScriptableEntity*> Scripts;
	};

}
