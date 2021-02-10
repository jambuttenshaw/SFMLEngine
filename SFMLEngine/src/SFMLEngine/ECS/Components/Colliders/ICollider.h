#pragma once


#include "../../System.h"


namespace SFMLEngine {
	
	struct BoxCollider;
	struct CircleCollider;

	enum class ColliderType
	{
		Invalid = 0, Box, Circle
	};

	struct ICollider
	{
		friend class System;
		virtual ~ICollider() = default;

		virtual bool Colliding(const BoxCollider& other) abstract;
		virtual bool Colliding(const CircleCollider& other) abstract;

		ColliderType Type;

	private:
		bool m_Modified = false;
	};

}
