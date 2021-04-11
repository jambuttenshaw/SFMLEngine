#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEngine/ECS/System.h"


namespace SFMLEngine {

	class Rigidbody
	{
	public:
		friend class System;

		

		Rigidbody()
			: m_Velocity(), m_Acceleration(), m_Mass(1), m_Force()
		{}

		Rigidbody(float mass)
			: m_Velocity(), m_Acceleration(), m_Mass(mass), m_Force()
		{}

		Rigidbody(float mass, const sf::Vector2f& velocity, const sf::Vector2f& acceleration)
			: m_Velocity(velocity), m_Acceleration(acceleration), m_Mass(mass), m_Force()
		{}

		inline void SetPosition(const sf::Vector2f& pos) { m_Position = pos; }
		inline void ChangePosition(const sf::Vector2f& delta) { m_Position += delta; }
		inline const sf::Vector2f& GetPosition() const { return m_Position; }
		
		inline void SetOldPosition(const sf::Vector2f& pos) { m_OldPosition = pos; }
		inline const sf::Vector2f& GetOldPosition() const { return m_OldPosition; }
		
		inline void SetVelocity(const sf::Vector2f& vel) { m_Velocity = vel; }
		inline void ChangeVelocity(const sf::Vector2f& delta) { m_Velocity += delta; }
		inline const sf::Vector2f& GetVelocity() const { return m_Velocity; }
		
		inline void SetAcceleration(const sf::Vector2f& acc) { m_Acceleration = acc; }
		inline void ChangeAcceleration(const sf::Vector2f& delta) { m_Acceleration += delta; }
		inline const sf::Vector2f& GetAcceleration() const { return m_Acceleration; }
		
		inline void SetForce(const sf::Vector2f& force) { m_Force = force; }
		inline void ChangeForce(const sf::Vector2f& delta) { m_Force += delta; }
		inline const sf::Vector2f& GetForce() const { return m_Force; }

		inline void SetMass(float mass) { m_Mass = mass; }
		inline float GetMass() const { return m_Mass; }

	private:
		bool m_Modified = false;

		sf::Vector2f m_Position;
		sf::Vector2f m_OldPosition;
		sf::Vector2f m_Velocity;
		sf::Vector2f m_Acceleration;
		sf::Vector2f m_Force;
		float m_Mass;
	};


}
