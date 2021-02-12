#include "BoxCollider.h"

#include "CircleCollider.h"
#include "TilemapCollider.h"

#include "../../../Log.h"
#include "../../../Math.h"

#include "../../../Renderer/Renderer.h"


namespace SFMLEngine {

	CollisionData BoxCollider::Colliding(BoxCollider& other, const sf::Vector2f& otherPos)
	{
		ZoneScoped;

		// offset is the pos of this collider
		// otherPos + other.Offset is the pos of the second collider
		sf::Vector2f otherOffsetPos = other.Offset + otherPos;

		bool overlapX = Offset.x + Size.x >= otherOffsetPos.x && otherOffsetPos.x + other.Size.x >= Offset.x;
		bool overlapY = Offset.y + Size.y >= otherOffsetPos.y && otherOffsetPos.y + other.Size.y >= Offset.y;

		return CollisionData{ overlapX && overlapY, sf::FloatRect{ Offset, Size } };
	}

	CollisionData BoxCollider::Colliding(CircleCollider& other, const sf::Vector2f& otherPos)
	{
		sf::Vector2f circleCentre = other.Offset + otherPos + sf::Vector2f{other.Radius, other.Radius};

		sf::Vector2f half_extents{ Size * 0.5f };
		sf::Vector2f centre{ Offset + half_extents };

		sf::Vector2f diff = centre - circleCentre + Clamp(circleCentre - centre, -half_extents, half_extents);

		return CollisionData{ SquareMagnitude(diff) <= other.Radius * other.Radius, sf::FloatRect() };
	}

	CollisionData BoxCollider::Colliding(TilemapCollider& other, const sf::Vector2f& otherPos)
	{
		// tilemap vs box collider collision
		ZoneScoped;

		return other.Colliding(*this, otherPos);
	}

	void BoxCollider::DrawDebug()
	{
		Renderer::DrawDebugRect(Offset, Size, sf::Color::Green);
	}

}
