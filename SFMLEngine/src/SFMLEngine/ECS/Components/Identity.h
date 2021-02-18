#pragma once

#include "SFMLEngine/ECS/System.h"


namespace SFMLEngine {

	struct Identity
	{
		friend class System;

		std::string Name;
		std::string Tag;
		Layer EntityLayer;

		Identity()
			: Name(), Tag("NULL"), EntityLayer()
		{}
		Identity(const std::string& name)
			: Name(name), Tag("NULL"), EntityLayer()
		{}
		Identity(const std::string& name, const std::string& tag, Layer layer)
			: Name(name), Tag(tag), EntityLayer(layer)
		{}

	private:
		bool m_Modified = false;
	};

}
