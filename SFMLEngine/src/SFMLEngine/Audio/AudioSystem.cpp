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


	std::unordered_map<std::string, ResourceID> AudioSystem::s_Sounds;

	void AudioSystem::Shutdown()
	{
		for (auto& sound : s_Sounds)
		{
			ResourceManager::DeleteResource<SoundResource>(sound.second);
		}
		s_Sounds.clear();
	}

	ResourceID AudioSystem::LoadSound(const std::string& soundHandle, const std::string& filepath)
	{
		SFMLE_CORE_ASSERT(s_Sounds.find(soundHandle) == s_Sounds.end(), "A sound already exists with that handle!");


		SoundResource* newSound = new SoundResource(filepath);
		ResourceID newID = ResourceManager::ManageResource(newSound);

		s_Sounds.insert({ soundHandle, newID });
	}

	void AudioSystem::DeleteSound(const std::string& soundHandle)
	{
		SFMLE_CORE_ASSERT(s_Sounds.find(soundHandle) != s_Sounds.end(), "No sound exists with that handle!");

		ResourceManager::DeleteResource<SoundResource>(s_Sounds[soundHandle]);
		s_Sounds.erase(soundHandle);
	}

	void AudioSystem::PlaySound(const std::string& soundHandle)
	{
		SFMLE_CORE_ASSERT(s_Sounds.find(soundHandle) != s_Sounds.end(), "No sound exists with that handle!");
		ResourceManager::GetResourceHandle<SoundResource>(s_Sounds[soundHandle])->GetSoundObject().play();
	}

	void AudioSystem::StopSound(const std::string& soundHandle)
	{
		SFMLE_CORE_ASSERT(s_Sounds.find(soundHandle) != s_Sounds.end(), "No sound exists with that handle!");
		ResourceManager::GetResourceHandle<SoundResource>(s_Sounds[soundHandle])->GetSoundObject().stop();
	}

	void AudioSystem::StopAllSounds()
	{
		for (auto& sound : s_Sounds)
		{
			ResourceManager::GetResourceHandle<SoundResource>(sound.second)->GetSoundObject().stop();
		}
	}

}
