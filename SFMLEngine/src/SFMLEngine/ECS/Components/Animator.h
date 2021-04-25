#pragma once

#include "SFMLEngine/ECS/System.h"

#include "SFMLEngine/Core.h"

#include <SFML/Graphics.hpp>
#include <unordered_map>


namespace SFMLEngine {

	class AnimationFrame
	{
	public:

		AnimationFrame()
			: m_ImageRect(), m_Offset()
		{}
		AnimationFrame(const sf::IntRect& rect)
			: m_ImageRect(rect), m_Offset()
		{}
		AnimationFrame(const sf::IntRect& rect, const sf::Vector2f& offset)
			: m_ImageRect(rect), m_Offset(offset)
		{}

		AnimationFrame(int x, int y, int w, int h)
			: m_ImageRect(x, y, w, h), m_Offset()
		{}
		AnimationFrame(int x, int y, int w, int h, float oX, float oY)
			: m_ImageRect(x, y, w, h), m_Offset(oX, oY)
		{}


		inline const sf::IntRect& GetRect() const { return m_ImageRect; }
		inline const sf::Vector2f& GetOffset() const { return m_Offset; }

	private:
		sf::IntRect m_ImageRect;
		sf::Vector2f m_Offset;
	};

	class Animation
	{
	public:

		Animation()
		{}
		Animation(const std::string& name, std::initializer_list<AnimationFrame> frames, float speed = 1.0f)
			: m_Name(name), m_Frames(frames), m_AnimationSpeed(speed)
		{}

		inline void AddFrame(sf::IntRect frame)
		{
			m_Frames.push_back(frame);
		}


		void Animate(float ts)
		{
			if (m_Playing)
			{
				m_ElapsedTime += ts;
				if (m_ElapsedTime >= m_AnimationSpeed)
				{
					m_FrameIndex++;
					if (m_FrameIndex >= m_Frames.size())
					{
						if (m_Looping)
						{
							m_FrameIndex = 0;
						}
						else
						{
							m_FrameIndex--;
							m_Playing = false;
						}
					}
					m_CurrentFrame = &m_Frames[m_FrameIndex];
					m_ElapsedTime = 0;
				}
			}
		}

		inline void Reset()
		{
			m_ElapsedTime = 0;
			m_FrameIndex = 0;
			m_CurrentFrame = &m_Frames[m_FrameIndex];
			m_Playing = true;
		}


		inline const std::string& GetName() const { return m_Name; }
		inline const AnimationFrame* const GetCurrentFrame() const { return m_CurrentFrame; }

		inline bool IsFlipped() const { return m_Flipped; }
		inline void SetFlipped(bool flipped) { m_Flipped = flipped; }

		inline bool IsLooping() const { return m_Looping; }
		inline void SetLooping(bool looping) { m_Looping = looping; }

		inline bool IsPlaying() const { return m_Playing; }
		inline void SetPlaying(bool playing) { m_Playing = playing; }

	private:
		std::string m_Name;

		AnimationFrame* m_CurrentFrame = nullptr;
		size_t m_FrameIndex = 0;
		std::vector<AnimationFrame> m_Frames;

		bool m_Flipped = false;
		bool m_Looping = true;
		bool m_Playing = true;

		float m_AnimationSpeed = 1.0f;
		float m_ElapsedTime = 0;


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


		inline AnimableType GetAnimableType() const { return m_ObjectType; }
		

		inline bool GetFlipped() const { return m_Flip; }
		inline void SetFlipped(bool flip) { m_Flip = flip; }


		void AddAnimation(Animation anim)
		{
			SFMLE_CORE_ASSERT(m_Animations.find(anim.GetName()) == m_Animations.end(), "Animation with that name already exists!");
			SFMLE_CORE_ASSERT(anim.GetName() != "null", "Cannot create animation with that name!");
			m_Animations.insert(std::make_pair(anim.GetName(), anim));
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

		inline void Stop()
		{
			m_CurrentAnimation = "null";
		}
		inline void PauseMusic()
		{
			m_Animations[m_CurrentAnimation].SetPlaying(false);
		}
		inline void Resume()
		{
			m_Animations[m_CurrentAnimation].SetPlaying(true);
		}

		inline const std::string& GetCurrentAnimationName() const { return m_CurrentAnimation; }

		inline Animation& GetCurrentAnimation()
		{
			return m_Animations[m_CurrentAnimation];
		}
		inline const AnimationFrame& GetCurrentFrame()
		{
			return *m_Animations[m_CurrentAnimation].GetCurrentFrame();
		}


		inline void Reset()
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
