#include "pch.h"
#include "SoundManager.h"
#include "Entity.h"
#include "Utils.h"
#include "json.hpp"

SoundManager::SoundManager()
{
	m_sounds.reserve(64); // Réserver de la mémoire pour 64 sons afin d'éviter les reallocations fréquentes lors de l'ajout de sons
	m_soundsVector.reserve(64); // Réserver de la mémoire pour 64 sons afin d'éviter les reallocations fréquentes lors de l'ajout de sons
}

SoundManager::~SoundManager()
{
	for (auto& sound : m_soundsVector)
	{
		delete sound;
	}
	m_soundsVector.clear();
	m_sounds.clear();
}

SoundManager* SoundManager::Get()
{
	static SoundManager instance;
	return &instance;
}

void SoundManager::Update(float p_deltatime)
{
	for (Sound* sound : m_soundsVector)
	{
		if (sound->GetAudioFormat() == AudioFormat::WAV)
		{
			sound->RemoveOldTrack();
		}

		if (m_isSpatializationEnabled)
		{
			ApplySpatialization(sound, p_deltatime);
		}
	}
}

void SoundManager::AddSound(const std::string& p_path)
{
	std::ifstream file(p_path);
	if (!file.is_open())
	{
		SDL_Log("Lib/AssetManager.cpp/AddTexture/failed to open file: %s", p_path.c_str());
		return;
	}

	nlohmann::ordered_json sounds_json;
	file >> sounds_json;

	Vector2i wSize = Application::Get()->GetWindow()->GetWindowSize();

	for (auto& [categorie, sounds] : sounds_json.items())
	{
		for (auto& sound : sounds)
		{
			std::string path = sound["path"].get<std::string>();
			std::string autoId = Utils::GetNameFromPath(path);
			auto it = m_sounds.find(autoId);
			if (it != m_sounds.end())
			{
				//SDL_Log("Lib/AssetManager.cpp/AddTexture/GetNameFromPath/Sound already loaded with ID : %s", autoId.c_str());
				continue;
			}
			Sound* sound = new Sound(m_mixer, wSize);
			sound->Initialize(path, Vector2f(0, 0));
			sound->SetName(autoId);
			m_sounds[autoId] = m_soundsVector.size();
			m_soundsVector.push_back(sound);
		}
	}
}
void SoundManager::AddSound(const std::string& p_path, Vector2f p_position)
{
	std::string autoId = Utils::GetNameFromPath(p_path);
	if (m_sounds.find(autoId) == m_sounds.end())
	{
		Vector2i wSize = Application::Get()->GetWindow()->GetWindowSize();

		Sound* sound = new Sound(m_mixer, wSize);
		sound->Initialize(p_path, p_position);
		sound->SetName(autoId);
		m_sounds[autoId] = m_soundsVector.size();
		m_soundsVector.push_back(sound);
		SDL_Log("Engine/SoundManager.cpp/AddSound/GetNameFromPath/Sound load with ID : %s", autoId.c_str());
	}
}

void SoundManager::RemoveSound(const std::string& p_id)
{
	if (m_soundsVector.empty())
	{
		return;
	}
	auto it = m_sounds.find(p_id);

	if (it == m_sounds.end())
	{
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_soundsVector.size() - 1;

	if (indexToDelete != lastIndex)
	{
		std::swap(m_soundsVector[indexToDelete], m_soundsVector[lastIndex]);
		m_sounds[m_soundsVector[indexToDelete]->GetName()] = indexToDelete;
	}
	delete m_soundsVector[lastIndex];
	m_soundsVector[lastIndex] = nullptr;
	m_soundsVector.pop_back();
	m_sounds.erase(it);
}

void SoundManager::RemoveAllSounds()
{
	for (auto& sound : m_soundsVector)
	{
		delete sound;
	}
	m_soundsVector.clear();
	m_sounds.clear();
}

Sound* SoundManager::GetSound(const std::string& p_id) const
{
	auto it = m_sounds.find(p_id);
	if (it == m_sounds.end())
	{
		SDL_Log("Engine/SoundManager.cpp/GetSound/find/Sound with id '%s' not found", p_id.c_str());
		return nullptr;
	}

	return m_soundsVector[it->second];
}

void SoundManager::Play(const std::string& p_id)
{
	Sound* sound = GetSound(p_id);
	if (sound)
	{
		Vector2f listenerPos = (m_listener == nullptr)
		? (Vector2f)Application::Get()->GetWindow()->GetWindowSize() / 2.0f
		: m_listener->GetPosition();

		sound->Play(listenerPos);
	}
}

void SoundManager::Stop(const std::string& p_id)
{
	Sound* sound = GetSound(p_id);
	if (sound)
	{
		sound->Stop();
	}
}

void SoundManager::Pause(const std::string& p_id)
{
	Sound* sound = GetSound(p_id);
	if (sound)
	{
		sound->Pause();
	}
}

void SoundManager::Resume(const std::string& p_id)
{
	Sound* sound = GetSound(p_id);
	if (sound)
	{
		sound->Resume();
	}
}

void SoundManager::PauseAll()
{
	for (Sound* sound : m_soundsVector)
	{
		sound->Pause();
	}
}

void SoundManager::ResumeAll()
{
	for (Sound* sound : m_soundsVector)
	{
		sound->Resume();
	}
}

void SoundManager::StopAll()
{
	for (Sound* sound : m_soundsVector)
	{
		sound->Stop();
	}
}

void SoundManager::SetMusicVolume(int p_volume)
{
	m_MusiqueVolume = Utils::Clamp(static_cast<float>(p_volume) / 100.0f, 0.0f, 1.0f);

	for (Sound* sound : m_soundsVector)
	{
		if (sound == nullptr || sound->GetAudioFormat() != AudioFormat::MP3)
		{
			continue;
		}
		sound->UpdateRelativeVolume(m_MusiqueVolume);
	}
}

void SoundManager::SetSFXVolume(int p_volume)
{
	m_SFXVolume = Utils::Clamp(static_cast<float>(p_volume) / 100.0f, 0.0f, 1.0f);

	for (Sound* sound : m_soundsVector)
	{
		if (sound == nullptr || sound->GetAudioFormat() != AudioFormat::WAV)
		{
			continue;
		}
		sound->UpdateRelativeVolume(m_SFXVolume);
	}
}

void SoundManager::IncreaseMusicVolume(int p_amount)
{
	int currentVolume = static_cast<int>(m_MusiqueVolume * 100.0f);

	SetMusicVolume(currentVolume + p_amount);
}

void SoundManager::IncreaseSFXVolume(int p_amount)
{
	int currentVolume = static_cast<int>(m_SFXVolume * 100.0f);
	SetSFXVolume(currentVolume + p_amount);
}

void SoundManager::DecreaseMusicVolume(int p_amount)
{
	int currentVolume = static_cast<int>(m_MusiqueVolume * 100.0f);
	SetMusicVolume(currentVolume - p_amount);
}

void SoundManager::DecreaseSFXVolume(int p_amount)
{
	int currentVolume = static_cast<int>(m_SFXVolume * 100.0f);
	SetSFXVolume(currentVolume - p_amount);
}

int SoundManager::GetSFXVolume() const
{
	return static_cast<int>(m_SFXVolume * 100.0f);
}

int SoundManager::GetMusicVolume() const
{
	return static_cast<int>(m_MusiqueVolume * 100.0f);
}

#ifdef _DEBUG
void SoundManager::DebugVolume() const
{
	int sfxVolume = GetSFXVolume();
	int musicVolume = GetMusicVolume();
	SDL_Log("Engine/SoundManager.cpp/DebugVolume/Music \nVolume: %d, SFX Volume: %d", musicVolume, sfxVolume);
}
void SoundManager::DebugListenerPosition() const
{
	if (m_listener)
	{
		Vector2f pos = m_listener->GetPosition();
		SDL_Log("Engine/SoundManager.cpp/DebugListenerPosition/Listener Position: (%.2f, %.2f)", pos.GetX(), pos.GetY());
	}
	else
	{
		SDL_Log("Engine/SoundManager.cpp/DebugListenerPosition/No listener set");
	}
}
#endif

void SoundManager::SetSoundPosition(const std::string& p_id, Vector2f p_pos)
{
	Sound* sound = GetSound(p_id);
	if (sound)
	{
		sound->SetPosition(p_pos);
	}
}

Vector2f SoundManager::GetSoundPosition(const std::string& p_id) const
{
	Sound* sound = GetSound(p_id);
	if (sound)
	{
		return sound->GetPosition();
	}
	return Vector2f(0.0f, 0.0f);
}

void SoundManager::SetPause_Resume_Sound(const std::string& p_id)
{
	auto it = m_sounds.find(p_id);
	if (it == m_sounds.end())
	{
		SDL_Log("Engine/SoundManager.cpp/Stop/find/Sound with id '%s' not found", p_id.c_str());
		return;
	}
	if (it != m_sounds.end())
	{
		AudioFormat format = m_soundsVector[it->second]->GetAudioFormat();
		if (format == AudioFormat::WAV)
		{
			if (m_isPausedSFX)
			{
				m_soundsVector[it->second]->Resume();
				m_isPausedSFX = false;
			}
			else
			{
				m_soundsVector[it->second]->Pause();
				m_isPausedSFX = true;
			}
		}
		else if (format == AudioFormat::MP3)
		{
			if (m_isPausedMusic)
			{
				m_soundsVector[it->second]->Resume();
				m_isPausedMusic = false;
			}
			else
			{
				m_soundsVector[it->second]->Pause();
				m_isPausedMusic = true;
			}
		}
	}
}

void SoundManager::SetSpatializationEnabled_Disable()
{
	if (m_isSpatializationEnabled)
	{
		m_isSpatializationEnabled = false;
		SDL_Log("Engine/SoundManager.cpp/SetSpatializationEnabled_Disable/Spatialization disabled");
	}
	else
	{
		m_isSpatializationEnabled = true;
		SDL_Log("Engine/SoundManager.cpp/SetSpatializationEnabled_Disable/Spatialization enabled");
	}
}

void SoundManager::ApplySpatialization(Sound* p_sound, float p_deltaTime)
{
	if (m_isSpatializationEnabled && p_sound != nullptr)
	{
		Vector2f listenerPos = (m_listener == nullptr)
			? (Vector2f)Application::Get()->GetWindow()->GetWindowSize() / 2.0f
			: m_listener->GetPosition();

		p_sound->ApplySpatialization(listenerPos, p_deltaTime);
	}
}