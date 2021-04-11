#pragma once


#include "SFMLEngine/ECS/Components/Transform.h"
#include "SFMLEngine/ECS/System.h"


namespace SFMLEngine {
	
	// the possible different types of collider
	// Inavlid exists as to give a default value to initialize
	// members of type ColliderType with
	enum class ColliderType
	{
		Invalid = 0, Box, Circle, Tilemap
	};


	// because Components are accessed by their type,
	// and a collideable object can have one of many different colliders
	// we need a way for the collision system to know what type of collider
	// to retrieve from the entity.

	// this is done with a ColliderInfo component
	// it tells the collision system the type of the collider attached to an entity
	class ColliderInfo
	{
	public:
		friend class System;

		ColliderInfo()
			: m_Type(ColliderType::Invalid)
		{}
		ColliderInfo(ColliderType type)
			: m_Type(type)
		{}

		inline ColliderType GetType() const { return m_Type; }

	private:
		bool m_Modified = false;
		
		ColliderType m_Type;
	};



	// virtual base class for other colliders to inherit from
	// defines the interface for the collision system to interact with colliders
	// holds properties that all colliders can exhibit,
	// such as whether its a trigger, its unique identifier and a pointer to its transform
	struct Collider
	{
		friend class System;

		virtual ~Collider() {};

		void Init();

		inline virtual void SetTransform(Transform* transform) { m_Transform = transform; }

		virtual sf::FloatRect GetLocalBounds() const = 0;
		inline sf::FloatRect GetGlobalBounds() const { return m_Transform->GetLocalToWorldTransformMatrix().transformRect(GetLocalBounds()); }

		inline ColliderID GetColliderID() const { return m_ColliderID; }
		void SetNewColliderID();

		inline void SetTrigger(bool trigger) { m_IsTrigger = trigger; }
		inline bool IsTrigger() const { return m_IsTrigger; }

	protected:
		bool m_Modified = false;
		
		// whether the collider is just used as an event trigger
		// or whether it is also used for physics
		bool m_IsTrigger = false;

		Transform* m_Transform = nullptr;
		ColliderID m_ColliderID = NULL_COLLIDER_ID;
	};

}
