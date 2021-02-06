#pragma once

#include "../System.h"
#include "../ScriptableEntity.h"

#include <unordered_map>

namespace SFMLEngine {

	struct NativeScripts
	{
		friend class System;

		std::unordered_map<const char*, ScriptableEntity*> Scripts;

	private:
		bool m_Modified = false;
	};

}

