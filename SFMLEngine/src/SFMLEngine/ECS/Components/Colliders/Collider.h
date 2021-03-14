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

		void Init();

		virtual void SetTransform(Transform* transform) { m_Transform = transform; }

		virtual sf::FloatRect GetLocalBounds() const = 0;
		sf::FloatRect GetGlobalBounds() const { return m_Transform->GetLocalToWorldTransformMatrix().transformRect(GetLocalBounds()); }

		ColliderID GetColliderID() const { return m_ColliderID; }
		void SetNewColliderID();

	protected:
		bool m_Modified = false;

		Transform* m_Transform = nullptr;
		ColliderID m_ColliderID = NULL_COLLIDER_ID;
	};

}
