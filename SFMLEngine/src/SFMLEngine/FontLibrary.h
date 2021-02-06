#pragma once

#include <SFML/Graphics.hpp>

#include "Constants.h"

#include <unordered_map>


namespace SFMLEngine {

	class FontLibrary
	{
	public:
		static void Init();
		static void Shutdown();

		static ResourceID LoadNewFont(const std::string& name, const std::string& filepath);
		static ResourceID GetFontID(const std::string& name);

	private:
		static std::unordered_map<std::string, ResourceID> s_FontLibrary;
	};

}
