#pragma once

#include "SFMLEngine/ECS/System.h"
#include "SFMLEngine/ECS/ScriptableEntity.h"

#include <unordered_map>

namespace SFMLEngine {

	struct NativeScripts
	{
		friend class System;

		std::unordered_map<const char*, ScriptableEntity*> Scripts;

	private:
		bool m_Modified = true;
	};

}

