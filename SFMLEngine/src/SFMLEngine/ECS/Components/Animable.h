#pragma once

#include <SFML/Graphics.hpp>
#include "Animator.h"

namespace SFMLEngine {

	class Animable
	{
	public:
		virtual ~Animable() = default;

		virtual void SetFrame(const AnimationFrame& frame, bool flipped) = 0;
	};

}
