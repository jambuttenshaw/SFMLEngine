#pragma once

namespace SFMLEngine {

	class Component
	{
	public:
		enum ComponentType {None = 0, Transform, SpriteRenderer};

	public:
		virtual ~Component() = 0;

	protected:
		ComponentType m_Type;
	};

	class TransformComponent : public Component
	{
	public:
		TransformComponent();
		~TransformComponent();
	};

	class SpriteRendererComponent : public Component
	{
	public:
		SpriteRendererComponent();
		~SpriteRendererComponent();
	};

}

