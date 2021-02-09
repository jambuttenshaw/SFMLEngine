#pragma once

#include <SFML/Graphics.hpp>

#include "../System.h"


namespace SFMLEngine {

	struct Rigidbody
	{
		friend class System;

		sf::Vector2f Velocity;
		sf::Vector2f Acceleration;
		float Mass;

		Rigidbody()
			: Velocity(), Acceleration(), Mass(1)
		{}

		Rigidbody(float mass)
			: Velocity(), Acceleration(), Mass(mass)
		{}

		Rigidbody(float mass, const sf::Vector2f& velocity, const sf::Vector2f& acceleration)
			: Velocity(velocity), Acceleration(acceleration), Mass(mass)
		{}

	private:
		bool m_Modified = false;
	};


}
