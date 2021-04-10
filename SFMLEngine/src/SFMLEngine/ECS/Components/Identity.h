#pragma once

#include "SFMLEngine/ECS/System.h"


namespace SFMLEngine {

	class Identity
	{
	public:
		friend class System;

		Identity()
			: m_Name(), m_Tag("NULL"), m_Layer()
		{}
		Identity(const std::string& name)
			: m_Name(name), m_Tag("NULL"), m_Layer()
		{}
		Identity(const std::string& name, const std::string& tag, Layer layer)
			: m_Name(name), m_Tag(tag), m_Layer(layer)
		{}

		inline const std::string& GetName() const { return m_Name; }
		inline void SetName(const std::string& name) { m_Name = name; }

		inline const std::string& GetTag() const { return m_Tag; }
		inline void SetTag(const std::string& tag) { m_Tag = tag; }

		inline Layer GetLayer() const { return m_Layer; }
		inline void SetLayer(Layer layer) { m_Layer = layer; }

	private:
		bool m_Modified = false;

		std::string m_Name;
		std::string m_Tag;
		Layer m_Layer;
	};

}
