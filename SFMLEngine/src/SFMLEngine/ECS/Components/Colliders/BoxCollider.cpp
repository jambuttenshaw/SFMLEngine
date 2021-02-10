#include "BoxCollider.h"

#include "CircleCollider.h"

#include "../../../Log.h"

namespace SFMLEngine {

	bool BoxCollider::Colliding(const BoxCollider& other, const sf::Vector2f& otherPos)
	{
		// offset is the pos of this collider
		// otherPos + other.Offset is the pos of the second collider
		sf::Vector2f otherOffsetPos = other.Offset + otherPos;

		bool overlapX = Offset.x + Size.x >= otherOffsetPos.x && otherOffsetPos.x + other.Size.x >= Offset.x;
		bool overlapY = Offset.y + Size.y >= otherOffsetPos.y && otherOffsetPos.y + other.Size.y >= Offset.y;

		return overlapX && overlapY;
	}

	bool BoxCollider::Colliding(const CircleCollider& other, const sf::Vector2f& otherPos)
	{
		return false;
	}

}
