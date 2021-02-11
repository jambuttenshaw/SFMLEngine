#pragma once

#include <SFML/Graphics.hpp>

#include "../System.h"


namespace SFMLEngine {

	struct Rigidbody
	{
		friend class System;

		sf::Vector2f Velocity;
		sf::Vector2f Acceleration;
		sf::Vector2f Force;
		float Mass;

		Rigidbody()
			: Velocity(), Acceleration(), Mass(1), Force()
		{}

		Rigidbody(float mass)
			: Velocity(), Acceleration(), Mass(mass), Force()
		{}

		Rigidbody(float mass, const sf::Vector2f& velocity, const sf::Vector2f& acceleration)
			: Velocity(velocity), Acceleration(acceleration), Mass(mass), Force()
		{}

	private:
		bool m_Modified = false;
	};


}
