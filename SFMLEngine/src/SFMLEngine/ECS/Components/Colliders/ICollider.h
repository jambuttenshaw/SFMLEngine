#pragma once


#include "../../System.h"


namespace SFMLEngine {

	struct Collision
	{
		bool Collided;
		Entity Other;
	};

	enum class ColliderType
	{
		Invalid = 0, Box, Circle
	};

	struct ICollider
	{
		friend class System;

		ColliderType Type;

	private:
		bool m_Modified = false;
	};

}
