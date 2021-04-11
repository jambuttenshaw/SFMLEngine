#pragma once

#include "SFMLEngine/ECS/System.h"
#include "SFMLEngine/ECS/ScriptableEntity.h"

#include <unordered_map>

namespace SFMLEngine {

	class NativeScripts
	{
		using ScriptMap = std::unordered_map<const char*, ScriptableEntity*>;

	public:
		friend class System;

		inline void AddScript(const std::pair<const char*, ScriptableEntity*> script) { m_Scripts.insert(script); }
		
		inline const ScriptMap& GetScripts() const { return m_Scripts; }
		inline ScriptableEntity* GetScript(const char* scriptName) { return m_Scripts[scriptName]; }
		
		inline void DeleteScript(const char* scriptName) { m_Scripts.erase(scriptName); }
		inline void ClearScripts() { m_Scripts.clear(); }

		inline bool HasScript(const char* scriptName) const { return m_Scripts.find(scriptName) != m_Scripts.end(); }

	private:
		bool m_Modified = true;

		 ScriptMap m_Scripts;
	};

}

