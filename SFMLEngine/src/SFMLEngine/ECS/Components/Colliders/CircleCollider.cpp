#include "CircleCollider.h"

#include "BoxCollider.h"

namespace SFMLEngine {

	bool CircleCollider::Colliding(const CircleCollider& other, const sf::Vector2f& otherPos)
	{
		return false;
	}

	bool CircleCollider::Colliding(const BoxCollider& other, const sf::Vector2f& otherPos)
	{
		return false;
	}

}
