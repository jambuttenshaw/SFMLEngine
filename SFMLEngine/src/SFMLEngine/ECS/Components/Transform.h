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


		Transform()
			: Position(sf::Vector2f(0, 0)), Rotation(0), Scale(sf::Vector2f(1, 1))
		{}
		Transform(const sf::Vector2f& pos)
			: Position(pos), Rotation(0), Scale(sf::Vector2f(1, 1))
		{}
		Transform(const sf::Vector2f& pos, float rot, const sf::Vector2f& scale)
			: Position(pos), Rotation(rot), Scale(scale)
		{}

		sf::Transform GetTransformMatrix() const
		{
			sf::Transform t;
			t.translate(Position);
			t.rotate(Rotation);
			t.scale(Scale);
			return t;
		}

	private:
		bool m_Modified = true;
	};

}

