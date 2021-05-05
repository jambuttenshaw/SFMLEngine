#pragma once

#include <SFML/Graphics.hpp>
#include "SFMLEngine/Core.h"
#include <random>



namespace std
{
	// allows sf::Vector2's to be used as keys in std::unordered_map's
	template<typename T> struct hash<sf::Vector2<T>>
	{
		std::size_t operator()(const sf::Vector2<T>& vec) const noexcept
		{
			// hash a vector2 to produce a scalar value
			// the idea is that every unique vector should produce a unique corresponding scalar
			// thus why prime numbers are used in the calculation

			// casts the vectors components to 64-bit unsigned type
			return (static_cast<size_t>(vec.x) * 73856093) ^ (static_cast<size_t>(vec.y) * 19349663);
		}
	};
}



namespace SFMLEngine {

	class Math
	{
	public:

		// sets up the rng system
		static void Init();
		static void Init(unsigned int seed);
		// deletes the rng system
		static void Shutdown();

		/*
		ENUMS
		*/

		// helpful representation of directions
		enum class Direction
		{
			Up, Down, Left, Right
		};

		/*
		CONSTANTS
		*/

		// multiply by these constants to convert between radians and degrees
		static inline const float RAD_TO_DEG = 57.2958f;
		static inline const float DEG_TO_RAD = 0.0174533f;

		// comes in lemon and apple varieties among others
		static inline const float PI = 3.1415926f;

		// direction vectors
		struct UnitVector {
			static inline const sf::Vector2f Left{ -1,  0 };
			static inline const sf::Vector2f Right{ 1,  0 };
			static inline const sf::Vector2f Up{ 0, -1 };
			static inline const sf::Vector2f Down{ 0,  1 };
		};


		/*
		GENERALLY HELPFUL FUNCTIONS
		*/
		// returns 1, 0 or -1 depending on the sign of the number
		// works on any type that can be constructed from integer 0
		// and is orderable
		template <typename T>
		static int Sign(T value)
		{
			return (T(0) < value) - (value < T(0));
		}

		// very helpful for debugging
		// returns a string based on the integral direction that is passed in
		static std::string DirectionToString(Direction dir)
		{
			switch (dir)
			{
			case Direction::Down:	return "down";     break;
			case Direction::Up:		return "up";       break;
			case Direction::Left:	return "left";     break;
			case Direction::Right:	return "right";    break;
			default:				return "invalid";  break;
			}
		}


		/*
		VECTOR MATH
		*/


		// max and min of 2D and 3D vectors

		static sf::Vector2f Max(const sf::Vector2f& a, const sf::Vector2f& b);
		static sf::Vector3f Max(const sf::Vector3f& a, const sf::Vector3f& b);
		static sf::Vector2f Min(const sf::Vector2f& a, const sf::Vector2f& b);
		static sf::Vector3f Min(const sf::Vector3f& a, const sf::Vector3f& b);

		// component-wise absolute value of a 2D vector
		static sf::Vector2f Abs(const sf::Vector2f& a);
		// component-wise absolute value of a 3D vector
		static sf::Vector3f Abs(const sf::Vector3f& a);

		// dot product of two 2D vectors
		static float Dot(const sf::Vector2f& a, const sf::Vector2f& b);
		// dot product of two 3D vectors
		static float Dot(const sf::Vector3f& a, const sf::Vector3f& b);

		// normalize a 2D vector
		static sf::Vector2f Normalize(const sf::Vector2f& a);
		// normalize a 3D vector
		static sf::Vector3f Normalize(const sf::Vector3f& a);

		// clamp a float between a max and a min value
		static float Clamp(float value, float min, float max);
		// clamp a 2D vector inside a rectangle
		static sf::Vector2f Clamp(const sf::Vector2f& value, const sf::Vector2f& min, const sf::Vector2f& max);
		// clamp a 3D vector inside a rectangular prism
		static sf::Vector3f Clamp(const sf::Vector3f& value, const sf::Vector3f& min, const sf::Vector3f& max);

		// find the square magnitude of a 2D vector
		static float SquareMagnitude(const sf::Vector2f& a);
		// find the square magnitude of a 3D vector
		static float SquareMagnitude(const sf::Vector3f& a);
		// find the magnitude of a 2D vector
		static float Magnitude(const sf::Vector2f& a);
		// find the magnitude of a 3D vector
		static float Magnitude(const sf::Vector3f& a);

		// return a value of type T that is t percent between A and B
		template<typename T>
		static T Lerp(const T& a, const T& b, float t) { return static_cast<T>((1 - t) * a + t * b); }

		// get direction not implemented for Vector3
		// returns the cardinal direction that the vector 'a' most closely resembles
		static Direction GetDirection(const sf::Vector2f& a);


		/*
		RECTANGLES
		*/
		// find the intersection between two rectangles
		static sf::FloatRect Intersection(const sf::FloatRect& a, const sf::FloatRect& b);
		// find the centroid of a rectangle
		static sf::Vector2f Centroid(const sf::FloatRect& a);
		// find the topleft and bottom right corner of a rectangle
		static std::pair<sf::Vector2f, sf::Vector2f> GetCorners(const sf::FloatRect& a);

		// flips a rectangle over its Y axis, where its origin is considered to be its topleft
		template <typename T>
		static sf::Rect<T> FlipRect(const sf::Rect<T>& rect)
		{
			return { rect.left + rect.width, rect.top, -rect.width, rect.height };
		}


		/*
		COLORS
		*/

		// linearly interpolate between the components of color A and the components of color T
		static sf::Color ColorLerpComponents(const sf::Color& a, const sf::Color& b, float t);


		/*
		RANDOM NUMBERS
		*/
		// generate a random integer between 0 and max
		static int RandomInt(int max);
		// generate a random integer between min and max
		static int RandomInt(int min, int max);

		// generate a random float between 0 and max
		static float RandomFloat(float max);
		// generate a random float between min and max
		static float RandomFloat(float min, float max);
		// generate a random float between 0 and 1
		static float RandomNormalizedFloat();

		// generate a random vector with magnitude 1 and direction between 0 and 2 PI radians
		static sf::Vector2f RandomUnitVector();

	private:
		// the pseudo random number generator used
		static std::mt19937* s_RandomNumberGenerator;
		// normalized floats are always in the same range so we can pre-define a uniform distribution of them
		static std::uniform_real_distribution<float> s_NormamlizedFloatDistribution;

	};
}
