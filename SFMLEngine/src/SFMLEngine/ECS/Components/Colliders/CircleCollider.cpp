#include "CircleCollider.h"

#include "BoxCollider.h"

namespace SFMLEngine {

	bool CircleCollider::Colliding(const CircleCollider& other)
	{
		return false;
	}

	bool CircleCollider::Colliding(const BoxCollider& other)
	{
		return false;
	}

}
