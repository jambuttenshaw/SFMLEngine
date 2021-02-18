#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/System.h"


namespace SFMLEngine {

	struct Transform
	{
		friend class System;

		sf::Vector2f Position;
		float Rotation = 0.0f;
		sf::Vector2f Scale;

		Transform* Parent = nullptr;

		Transform()
			: Position(), Rotation(0), Scale(sf::Vector2f(1, 1))
		{}
		Transform(const sf::Vector2f& pos)
			: Position(pos), Rotation(0), Scale(sf::Vector2f(1, 1))
		{}
		Transform(const sf::Vector2f& pos, float rot, const sf::Vector2f& scale)
			: Position(pos), Rotation(rot), Scale(scale)
		{}

		Transform(Transform* parent)
			: Position(), Rotation(0), Scale(sf::Vector2f(1, 1)), Parent(parent)
		{}
		Transform(const sf::Vector2f& pos, Transform* parent)
			: Position(pos), Rotation(0), Scale(sf::Vector2f(1, 1)), Parent(parent)
		{}


		sf::Transform GetLocalTransformMatrix() const
		{
			sf::Transform t;
			t.translate(Position);
			t.rotate(Rotation);
			t.scale(Scale);
			return t;
		}

		sf::Transform GetWorldTransformMatrix() const
		{
			if (Parent)
			{
				return GetLocalTransformMatrix().combine(Parent->GetWorldTransformMatrix());
			}
			else
			{
				return GetLocalTransformMatrix();
			}
		}

		sf::Vector2f GetWorldPosition() const
		{
			if (Parent)
			{
				return Position + Parent->GetWorldPosition();
			}
			else
			{
				return Position;
			}
		}

	private:
		bool m_Modified = true;
	};

}

