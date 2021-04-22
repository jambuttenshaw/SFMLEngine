#pragma once

#include <SFML/Audio.hpp>
#include "SFMLEngine/ResourceManagement/ResourceManager.h"


namespace SFMLEngine {


	// a class that describes a single sound
	class SoundResource
	{
	public:
		SoundResource(const std::string& filename);


		inline const std::string& GetFilepath() const { return m_Filepath; }
		inline sf::Sound& GetSoundObject() { return m_SoundObject; }

	private:
		std::string m_Filepath;

		ResourceID m_BufferID = NULL_RESOURCE_ID;
		sf::Sound m_SoundObject;
	};

	
	class AudioSystem
	{
	public:
		static void Shutdown();


		// load a sound from a file
		static ResourceID LoadSound(const std::string& soundHandle, const std::string& filepath);

		// delete a sound from memory
		static void DeleteSound(const std::string& soundHandle);



		static void PlaySound(const std::string& soundHandle);
		static void StopSound(const std::string& soundHandle);
		static void StopAllSounds();


	private:
		static std::unordered_map<std::string, ResourceID> s_Sounds;

	};

}
