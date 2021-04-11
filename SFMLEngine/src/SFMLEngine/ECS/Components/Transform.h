#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/System.h"


namespace SFMLEngine {

	class Transform
	{
	public:
		friend class System;


		Transform();
		Transform(const sf::Vector2f& pos);
		Transform(const sf::Vector2f& pos, float rot, const sf::Vector2f& scale);

		Transform(Transform* parent);
		Transform(const sf::Vector2f& pos, Transform* parent);


		sf::Transform GetLocalTransformMatrix() const;

		sf::Transform GetLocalToWorldTransformMatrix() const;
		sf::Transform GetWorldToLocalTransformMatrix() const;

		sf::Vector2f GetWorldPosition() const;

		sf::Vector2f LocalPointToWorldPoint(const sf::Vector2f& p);


		// getters and setters
		inline void SetPosition(const sf::Vector2f& pos) { m_Position = pos; }
		inline void Translate(const sf::Vector2f& delta) { m_Position += delta; }
		inline const sf::Vector2f& GetPosition() const { return m_Position; }

		inline void SetRotation(float rot) { m_Rotation = rot; }
		inline void Rotate(float delta) { m_Rotation += delta; }
		inline float GetRotation() const { return m_Rotation; }

		inline void SetScale(const sf::Vector2f& scale) { m_Scale = scale; }
		inline void Scale(const sf::Vector2f& delta) { m_Scale += delta; }
		inline const sf::Vector2f& GetScale() const { return m_Scale; }

		inline void SetParent(Transform* parent) { m_Parent = parent; }
		inline Transform* GetParent() const { return m_Parent; }

	private:
		bool m_Modified = true;

		sf::Vector2f m_Position;
		float m_Rotation = 0.0f;
		sf::Vector2f m_Scale;

		Transform* m_Parent = nullptr;
	};

}

