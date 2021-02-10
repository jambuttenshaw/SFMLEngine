#pragma once


#include "../../System.h"


namespace SFMLEngine {
	
	enum class ColliderType
	{
		Box = 0, Circle
	};

	struct Collider
	{
		friend class System;

		ColliderType Type;

	private:
		bool m_Modified = false;
	};

}
