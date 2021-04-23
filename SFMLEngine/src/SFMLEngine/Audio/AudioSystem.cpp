#include "AudioSystem.h"

#include "SFMLEngine/Core.h"


namespace SFMLEngine {



	SoundResource::SoundResource(const std::string& filename)
		: m_Filepath(filename)
	{
		// manage the sound buffer through the resource manager
		m_BufferID = ResourceManager::LoadFromFile<sf::SoundBuffer>(m_Filepath);

		// setup the sound object
		m_SoundObject.setBuffer(*ResourceManager::GetResourceHandle<sf::SoundBuffer>(m_BufferID));
	}

	SoundResource::~SoundResource()
	{
		ResourceManager::DeleteResource<sf::SoundBuffer>(m_BufferID);
	}


	std::unordered_map<std::string, SoundResource*> AudioSystem::s_Sounds;

	void AudioSystem::Shutdown()
	{
		for (auto& sound : s_Sounds)
		{
			delete sound.second;
		}
		s_Sounds.clear();
	}

	void AudioSystem::LoadSound(const std::string& soundHandle, const std::string& filepath)
	{
		// dont attempt to load a sound if its already in memory
		// we can safely silently ignore the request to load the sound
		if (s_Sounds.find(soundHandle) == s_Sounds.end())
		{
			SoundResource* newSound = new SoundResource(filepath);

			s_Sounds.insert({ soundHandle, newSound });
		}
	}

	void AudioSystem::DeleteSound(const std::string& soundHandle)
	{
		SFMLE_CORE_ASSERT(s_Sounds.find(soundHandle) != s_Sounds.end(), "No sound exists with that handle!");

		delete s_Sounds[soundHandle];
		s_Sounds.erase(soundHandle);
	}

	void AudioSystem::PlaySound(const std::string& soundHandle, bool forceReplay)
	{
		SFMLE_CORE_ASSERT(s_Sounds.find(soundHandle) != s_Sounds.end(), "No sound exists with that handle!");
		auto& sound = s_Sounds[soundHandle]->GetSoundObject();
		if (forceReplay)
			sound.play();
		else
		{
			if (sound.getStatus() != sound.Playing)
				sound.play();
		}
	}

	void AudioSystem::StopSound(const std::string& soundHandle)
	{
		SFMLE_CORE_ASSERT(s_Sounds.find(soundHandle) != s_Sounds.end(), "No sound exists with that handle!");
		s_Sounds[soundHandle]->GetSoundObject().stop();
	}

	void AudioSystem::StopAllSounds()
	{
		for (auto& sound : s_Sounds)
		{
			sound.second->GetSoundObject().stop();
		}
	}

	void AudioSystem::SetLooping(const std::string& soundHandle, bool flag)
	{
		SFMLE_CORE_ASSERT(s_Sounds.find(soundHandle) != s_Sounds.end(), "No sound exists with that handle!");
		s_Sounds[soundHandle]->GetSoundObject().setLoop(flag);
	}

	bool AudioSystem::IsLooping(const std::string& soundHandle)
	{
		SFMLE_CORE_ASSERT(s_Sounds.find(soundHandle) != s_Sounds.end(), "No sound exists with that handle!");
		return s_Sounds[soundHandle]->GetSoundObject().getLoop();
	}

}
