#pragma once

#include "SFMLEngine/ECS/System.h"

#include "SFMLEngine/Core.h"
#include "SFMLEngine/Timestep.h"

#include <SFML/Graphics.hpp>
#include <unordered_map>


namespace SFMLEngine {

	struct AnimationFrame
	{
		sf::IntRect ImageRect;
		sf::Vector2f Offset;

		AnimationFrame()
			: ImageRect(), Offset()
		{}
		AnimationFrame(const sf::IntRect& rect)
			: ImageRect(rect), Offset()
		{}
		AnimationFrame(const sf::IntRect& rect, const sf::Vector2f& offset)
			: ImageRect(rect), Offset(offset)
		{}

		AnimationFrame(int x, int y, int w, int h)
			: ImageRect(x, y, w, h), Offset()
		{}
		AnimationFrame(int x, int y, int w, int h, float oX, float oY)
			: ImageRect(x, y, w, h), Offset(oX, oY)
		{}
	};

	struct Animation
	{
		std::string Name;

		AnimationFrame* CurrentFrame = nullptr;
		size_t FrameIndex = 0;
		std::vector<AnimationFrame> Frames;

		bool Flipped = false;
		bool Looping = true;
		bool Playing = true;

		float AnimationSpeed = 1.0f;
		float ElapsedTime = 0;

		Animation()
		{}
		Animation(const std::string& name, std::initializer_list<AnimationFrame> frames, float speed = 1.0f)
			: Name(name), Frames(frames), AnimationSpeed(speed)
		{}
		Animation(const Animation& other)
		{
			Name = other.Name;
			CurrentFrame = other.CurrentFrame;
			FrameIndex = other.FrameIndex;
			Frames = other.Frames;

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
			Playing = true;
		}

	};

	

	enum class AnimableType
	{
		Invalid = 0, Sprite, GUIImage
	};



	class Animator
	{
	public:
		friend class System;

		Animator()
			: m_Animations()
		{}
		Animator(AnimableType type)
			: m_Animations(), m_ObjectType(type)
		{}


		AnimableType GetAnimableType() { return m_ObjectType; }
		

		bool GetFlipped() { return m_Flip; }
		void SetFlipped(bool flip) { m_Flip = flip; }


		void AddAnimation(Animation anim)
		{
			SFMLE_CORE_ASSERT(m_Animations.find(anim.Name) == m_Animations.end(), "Animation with that name already exists!");
			SFMLE_CORE_ASSERT(anim.Name != "null", "Cannot create animation with that name!");
			m_Animations.insert(std::make_pair(anim.Name, anim));
		}


		void SetCurrentAnimation(const std::string& name)
		{
			SFMLE_CORE_ASSERT(m_Animations.find(name) != m_Animations.end(), "Animation doesn't exist!");
			if (m_CurrentAnimation != name)
			{
				m_CurrentAnimation = name;
				m_Animations[m_CurrentAnimation].Reset();
			}
		}

		void Stop()
		{
			m_CurrentAnimation = "null";
		}
		void Pause()
		{
			m_Animations[m_CurrentAnimation].Playing = false;
		}
		void Resume()
		{
			m_Animations[m_CurrentAnimation].Playing = true;
		}

		const std::string& GetCurrentAnimationName() { return m_CurrentAnimation; }

		Animation& GetCurrentAnimation()
		{
			return m_Animations[m_CurrentAnimation];
		}
		const AnimationFrame& GetCurrentFrame()
		{
			return *m_Animations[m_CurrentAnimation].CurrentFrame;
		}


		void Reset()
		{
			for (auto& anim : m_Animations) anim.second.Reset();
		}

	private:
		bool m_Modified = false;

		std::unordered_map<std::string, Animation> m_Animations;
		std::string m_CurrentAnimation = "null";

		bool m_Flip = false;

		AnimableType m_ObjectType{ AnimableType::Invalid };
	};


}
