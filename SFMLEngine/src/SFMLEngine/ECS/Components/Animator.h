#pragma once

#include "SFMLEngine/ECS/System.h"

#include "SFMLEngine/Core.h"
#include "SFMLEngine/Timestep.h"

#include <SFML/Graphics.hpp>
#include <unordered_map>


namespace SFMLEngine {


	struct Animation
	{
		std::string Name;

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
		Animation(const std::string& name, std::initializer_list<sf::IntRect> frames)
			: Name(name), Frames(frames)
		{}
		Animation(const std::string& name, std::initializer_list<sf::IntRect> frames, float speed)
			: Name(name), Frames(frames), AnimationSpeed(speed)
		{}
		Animation(const Animation& other)
		{
			Name = other.Name;
			CurrentFrame = other.CurrentFrame;
			FrameIndex = other.FrameIndex;
			Frames = move(other.Frames);

			Flipped = other.Flipped;
			Looping = other.Looping;
			Playing = other.Playing;

			AnimationSpeed = other.AnimationSpeed;
			ElapsedTime = other.ElapsedTime;
		}

		void AddFrame(sf::IntRect frame)
		{
			Frames.push_back(frame);
		}


		void Animate(Timestep ts)
		{
			if (Playing)
			{
				ElapsedTime += ts;
				if (ElapsedTime >= AnimationSpeed)
				{
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
		}

		void Reset()
		{
			ElapsedTime = 0;
			FrameIndex = 0;
			CurrentFrame = &Frames[FrameIndex];
		}

	};


	struct Animator
	{
		friend class System;

		std::unordered_map<std::string, Animation> Animations;
		std::string CurrentAnimation;
		
		bool Flip = false;

		Animator()
			: Animations()
		{}
		Animator(std::unordered_map<std::string, Animation> anims)
			: Animations{ anims }
		{}
		Animator(const Animator& other)
		{
			Animations = move(other.Animations);
			Flip = other.Flip;

			SetCurrentAnimation(other.CurrentAnimation);
		}

		void AddAnimation(Animation anim)
		{
			Animations.insert(std::make_pair(anim.Name, anim));
		}


		void SetCurrentAnimation(const std::string& name)
		{
			SFMLE_CORE_ASSERT(Animations.find(name) != Animations.end(), "Animation doesn't exist!");
			CurrentAnimation = name;
		}
		Animation& GetCurrentAnimation()
		{
			return Animations[CurrentAnimation];
		}
		const sf::IntRect& GetCurrentFrame()
		{
			return *Animations[CurrentAnimation].CurrentFrame;
		}

	private:
		bool m_Modified = false;
	};


}
