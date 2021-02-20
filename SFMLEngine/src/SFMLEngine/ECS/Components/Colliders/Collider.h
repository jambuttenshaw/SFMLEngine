#pragma once


#include "SFMLEngine/ECS/Components/Transform.h"
#include "SFMLEngine/ECS/System.h"


namespace SFMLEngine {
	
	enum class ColliderType
	{
		Invalid = 0, Box, Circle, Tilemap
	};

	struct ColliderInfo
	{
		friend class System;

		ColliderType Type;

		ColliderInfo()
			: Type(ColliderType::Invalid)
		{}
		ColliderInfo(ColliderType type)
			: Type(type)
		{}

	private:
		bool m_Modified = false;
	};



	// virtual base class for other colliders to inherit from
	// defines the interface for the collision system to interact with colliders
	struct Collider
	{
		friend class System;

		virtual ~Collider() {};

		void SetTransform(Transform* transform) { m_Transform = transform; }

		virtual sf::FloatRect GetLocalBounds() const = 0;
		sf::FloatRect GetGlobalBounds() { return m_Transform->GetWorldTransformMatrix().transformRect(GetLocalBounds()); }

		/*
		virtual sf::Vector2f GetLocalOffset() const = 0;
		sf::Vector2f& GetGlobalOffset() { return m_Transform->GetWorldTransformMatrix().transformPoint(GetLocalOffset()); }
		*/

	protected:
		bool m_Modified = false;
		Transform* m_Transform = nullptr;
	};

}
