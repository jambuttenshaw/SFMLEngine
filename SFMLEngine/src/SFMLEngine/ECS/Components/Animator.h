#pragma once

#include "SFMLEngine/ECS/System.h"

#include "SFMLEngine/Core.h"
#include "SFMLEngine/Timestep.h"

#include <SFML/Graphics.hpp>
#include <unordered_map>


namespace SFMLEngine {


	struct Animation
	{
		sf::IntRect* CurrentFrame = nullptr;
		size_t FrameIndex = 0;
		std::vector<sf::IntRect> Frames;

		bool Flipped = false;
		bool Looping = true;
		bool Playing = true;

		float AnimationSpeed = 1.0f;
		float ElapsedTime = 0;

		Animation()
		{}
		Animation(std::initializer_list<sf::IntRect> frames)
			: Frames(frames)
		{}
		Animation(std::initializer_list<sf::IntRect> frames, float speed)
			: Frames(frames), AnimationSpeed(speed)
		{}

		void AddFrame(sf::IntRect frame)
		{
			Frames.push_back(frame);
		}


		bool Animate(Timestep ts)
		{
			bool changeFrame = false;
			if (Playing)
			{
				ElapsedTime += ts;
				if (ElapsedTime >= AnimationSpeed)
				{
					changeFrame = true;
					FrameIndex++;
					if (FrameIndex >= Frames.size())
					{
						if (Looping)
						{
							FrameIndex = 0;
						}
						else
						{
							FrameIndex--;
							Playing = false;
						}
					}
					CurrentFrame = &Frames[FrameIndex];
					ElapsedTime = 0;
				}
			}
			return changeFrame;
		}

		void Reset()
		{
			ElapsedTime = 0;
			FrameIndex = 0;
		}

	};


	struct Animator
	{
		friend class System;

		std::unordered_map<std::string, Animation> Animations;
		Animation* CurrentAnimation = nullptr;
		
		bool Flip = false;

		Animator()
			: Animations()
		{}
		Animator(std::unordered_map<std::string, Animation> anims)
			: Animations{ anims }
		{}

		void AddAnimation(std::pair<std::string, Animation> anim)
		{
			Animations.insert(anim);
		}
		void AddAnimation(const std::string& name, Animation anim)
		{
			Animations.insert(std::make_pair(name, anim));
		}


		void SetCurrentAnimation(const std::string& name)
		{
			SFMLE_CORE_ASSERT(Animations.find(name) != Animations.end(), "Animation doesn't exist!");
			CurrentAnimation = &Animations[name];
		}
		Animation& GetCurrentAnimation()
		{
			return *CurrentAnimation;
		}
		const sf::IntRect& GetCurrentFrame()
		{
			return *CurrentAnimation->CurrentFrame;
		}

	private:
		bool m_Modified = false;
	};


}
