#include "BoxCollider.h"

#include "CircleCollider.h"
#include "TilemapCollider.h"

#include "../../../Log.h"

namespace SFMLEngine {

	CollisionData BoxCollider::Colliding(BoxCollider& other, const sf::Vector2f& otherPos)
	{
		ZoneScoped;

		// offset is the pos of this collider
		// otherPos + other.Offset is the pos of the second collider
		sf::Vector2f otherOffsetPos = other.Offset + otherPos;

		bool overlapX = Offset.x + Size.x >= otherOffsetPos.x && otherOffsetPos.x + other.Size.x >= Offset.x;
		bool overlapY = Offset.y + Size.y >= otherOffsetPos.y && otherOffsetPos.y + other.Size.y >= Offset.y;

		return CollisionData{ overlapX && overlapY, Size };
	}

	CollisionData BoxCollider::Colliding(CircleCollider& other, const sf::Vector2f& otherPos)
	{
		return CollisionData{ false, sf::Vector2f() };
	}

	CollisionData BoxCollider::Colliding(TilemapCollider& other, const sf::Vector2f& otherPos)
	{
		// tilemap vs box collider collision
		ZoneScoped;

		return other.Colliding(*this, otherPos);
	}

}
