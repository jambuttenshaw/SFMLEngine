#pragma once

#include <SFML/Audio.hpp>
#include "SFMLEngine/ResourceManagement/ResourceManager.h"


namespace SFMLEngine {


	// a class that describes a single sound
	class SoundResource
	{
	private:
		struct CacheData
		{
			ResourceID ID;
			size_t Uses;
		};

	public:
		SoundResource(const std::string& filename);
		~SoundResource();

		void SetVolume(float volume);

		inline const std::string& GetFilepath() const { return m_Filepath; }
		inline sf::Sound& GetSoundObject() { return m_SoundObject; }

	private:
		std::string m_Filepath;

		ResourceID m_BufferID = NULL_RESOURCE_ID;
		sf::Sound m_SoundObject;


	private:
		static std::unordered_map<std::string, CacheData> s_BufferCache;
	};

	
	class AudioSystem
	{
	public:
		static void Shutdown();

		//////////////
		// LISTENER //
		//////////////


		static void SetListenerPosition(const sf::Vector2f& pos);
		static void SetListenerDirection(const sf::Vector2f& dir);


		////////////
		// SOUNDS //
		////////////

		// load a sound from a file
		static void LoadSound(const std::string& soundHandle, const std::string& filepath, float volume = 100);

		// delete a sound from memory
		static void DeleteSound(const std::string& soundHandle);

		static void PlaySound(const std::string& soundHandle, bool forceReplay = true);
		static void PlaySound(const std::string& soundHandle, const sf::Vector2f& position, bool forceReplay = true);
		static void StopSound(const std::string& soundHandle);
		static void StopAllSounds();

		static void SetLooping(const std::string& soundHandle, bool flag);
		static bool IsLooping(const std::string& soundHandle);

		static void SetRelativeToListener(const std::string& soundHandle, bool flag);

	private:
		static std::unordered_map<std::string, SoundResource*> s_Sounds;

	};

}
