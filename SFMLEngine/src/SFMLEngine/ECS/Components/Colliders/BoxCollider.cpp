#include "BoxCollider.h"

#include "CircleCollider.h"

namespace SFMLEngine {

	bool BoxCollider::Colliding(const BoxCollider& other)
	{
		return false;
	}

	bool BoxCollider::Colliding(const CircleCollider& other)
	{
		return false;
	}

}
