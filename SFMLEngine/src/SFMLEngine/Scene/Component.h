#pragma once

#include <SFML/Graphics.hpp>

namespace SFMLEngine {


	class Component
	{
	public:
		enum class ComponentType 
		{
			None = 0, Transform, SpriteRenderer
		};

	public:
		virtual ~Component() = 0;

		ComponentType GetType() { return m_Type; }


		static Component* Create(ComponentType type)
		{
			switch (type)
			{
				case ComponentType::Transform:			return new TransformComponent(); break;
				case ComponentType::SpriteRenderer:		return new SpriteRendererComponent(); break;
			}
		}

	protected:
		ComponentType m_Type = ComponentType::None;
	};





	class TransformComponent : public Component
	{
	public:
		TransformComponent(const sf::Vector2f& pos = { 0.0f, 0.0f }, float rotation = 0.0f, const sf::Vector2f& scale = { 1.0f, 1.0f })
			: Component(), m_Position(pos), m_Rotation(rotation), m_Scale(scale) 
		{
			m_Type = ComponentType::Transform;
		}
		~TransformComponent() = default;

		void SetPosition(const sf::Vector2f& pos) { m_Position = pos; }
		void SetRotation(float angle) { m_Rotation = angle; }
		void SetScale(const sf::Vector2f& scale) { m_Scale = scale; }

		void Translate(const sf::Vector2f& translation) { m_Position += translation; }
		void Rotate(float angle) { m_Rotation += angle; }
		void Scale(const sf::Vector2f& scale) { m_Scale += scale; }

		const sf::Vector2f& GetPosition() { return m_Position; }
		float GetRotation() { return m_Rotation; }
		const sf::Vector2f& GetScale() { return m_Scale; }

		const sf::Transform& GetTransform()
		{
			sf::Transform translation;
			translation.translate(m_Position);

			sf::Transform rotation;
			rotation.rotate(m_Rotation);

			sf::Transform scale;
			scale.scale(m_Scale);

			return translation * rotation * scale;
		}

	private:

		sf::Vector2f m_Position = { 0.0f, 0.0f };
		float m_Rotation = 0.0f;
		sf::Vector2f m_Scale = { 0.0f, 0.0f };
	};







	class SpriteRendererComponent : public Component
	{
	public:
		SpriteRendererComponent(const sf::Color& color = { 0.0, 0.0, 0.0, 1.0 })
			: Component(), m_Color(color)
		{
			m_Type = ComponentType::SpriteRenderer;
		}
		~SpriteRendererComponent() = default;

	private:
		sf::Color m_Color = { 0.0, 0.0, 0.0, 1.0 };
	};

}

