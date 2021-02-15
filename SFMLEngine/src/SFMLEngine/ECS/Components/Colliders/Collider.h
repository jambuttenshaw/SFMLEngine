#pragma once


#include "SFMLEngine/ECS/System.h"


namespace SFMLEngine {
	
	enum class ColliderType
	{
		Invalid = 0, Box, Circle, Tilemap
	};

	struct Collider
	{
		friend class System;

		ColliderType Type;

		Collider()
			: Type(ColliderType::Invalid)
		{}
		Collider(ColliderType type)
			: Type(type)
		{}

	private:
		bool m_Modified = false;
	};

}
