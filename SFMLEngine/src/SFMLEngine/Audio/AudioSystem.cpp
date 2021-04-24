#include "AudioSystem.h"

#include "SFMLEngine/Core.h"


namespace SFMLEngine {


	std::unordered_map<std::string, SoundResource::CacheData> SoundResource::s_BufferCache;

	SoundResource::SoundResource(const std::string& filename)
		: m_Filepath(filename)
	{
		if (s_BufferCache.find(filename) != s_BufferCache.end())
		{
			m_BufferID = s_BufferCache[filename].ID;
			s_BufferCache[filename].Uses++;
		}
		else
		{
			m_BufferID = ResourceManager::LoadFromFile<sf::SoundBuffer>(m_Filepath);
			s_BufferCache.insert({ filename, {m_BufferID, 1} });
		}
		// manage the sound buffer through the resource manager

		// setup the sound object
		m_SoundObject.setBuffer(*ResourceManager::GetResourceHandle<sf::SoundBuffer>(m_BufferID));

		m_SoundObject.setMinDistance(32);
	}

	SoundResource::~SoundResource()
	{
		s_BufferCache[m_Filepath].Uses--;
		if (s_BufferCache[m_Filepath].Uses == 0)
		{
			ResourceManager::DeleteResource<sf::SoundBuffer>(m_BufferID);
			s_BufferCache.erase(m_Filepath);
		}
	}

	void SoundResource::SetVolume(float volume)
	{
		m_SoundObject.setVolume(volume);
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

	void AudioSystem::SetListenerPosition(const sf::Vector2f& pos)
	{
		sf::Listener::setPosition(pos.x, 0.0f, pos.y);
	}

	void AudioSystem::SetListenerDirection(const sf::Vector3f& dir)
	{
		sf::Listener::setDirection(dir.x, dir.y, dir.z);
	}

	void AudioSystem::LoadSound(const std::string& soundHandle, const std::string& filepath, float volume)
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

	void AudioSystem::SetPosition(const std::string& soundHandle, const sf::Vector2f& pos)
	{
		SFMLE_CORE_ASSERT(s_Sounds.find(soundHandle) != s_Sounds.end(), "No sound exists with that handle!");
		s_Sounds[soundHandle]->GetSoundObject().setPosition(pos.x, 0.0f, pos.y);
	}

	void AudioSystem::SetRelativeToListener(const std::string& soundHandle, bool flag)
	{
		SFMLE_CORE_ASSERT(s_Sounds.find(soundHandle) != s_Sounds.end(), "No sound exists with that handle!");
		s_Sounds[soundHandle]->GetSoundObject().setRelativeToListener(flag);
	}

	void AudioSystem::SetMinimumDistance(const std::string& soundHandle, float dist)
	{
		SFMLE_CORE_ASSERT(s_Sounds.find(soundHandle) != s_Sounds.end(), "No sound exists with that handle!");
		s_Sounds[soundHandle]->GetSoundObject().setMinDistance(dist);
	}

	void AudioSystem::SetAttenuation(const std::string& soundHandle, float att)
	{
		SFMLE_CORE_ASSERT(s_Sounds.find(soundHandle) != s_Sounds.end(), "No sound exists with that handle!");
		s_Sounds[soundHandle]->GetSoundObject().setAttenuation(att);
	}

}
