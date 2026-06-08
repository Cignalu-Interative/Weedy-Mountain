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

void SoundManager::Update()
{
	for (Sound* sound : m_soundsVector)
	{
		if (sound->GetAudioFormat() == AudioFormat::WAV && m_isSpatializationEnabled)
		{
			ApplySpatialization(sound);
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

	for (auto& [categorie, sounds] : sounds_json.items())
	{
		for (auto& sound : sounds)
		{
			std::string path = sound["path"].get<std::string>();
			std::string autoId = Utils::GetNameFromPath(path);
			auto it = m_sounds.find(autoId);
			if (it != m_sounds.end())
			{
				SDL_Log("Lib/AssetManager.cpp/AddTexture/GetNameFromPath/Sound already loaded with ID : %s", autoId.c_str());
				continue;
			}
			Sound* sound = new Sound();
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
		Sound* sound = new Sound();
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
	else if (it != m_sounds.end())
	{
		return m_soundsVector[it->second];
	}
}

void SoundManager::Play(const std::string& p_id, int p_loops, SoundOrientation p_orientation, Uint8 p_distance)
{
	Sound* sound = GetSound(p_id);
	if (sound)
	{
		if (sound->GetAudioFormat() == AudioFormat::MP3)
		{
			sound->Play(p_loops);
			return;
		}
		Sint16 angle;
		if (p_orientation == SoundOrientation::NORTH)
		{
			angle = (Sint16)sound->GetAngle();
		}
		else
		{
			angle = (Sint16)p_orientation;
		}
		if (p_distance == 0)
		{
			p_distance = (Uint8)std::min(255.0f, sound->GetDistance());
		}
		sound->Play(p_loops, angle, p_distance);
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
	Mix_PauseMusic();
	Mix_Pause(-1);
}

void SoundManager::ResumeAll()
{
	Mix_ResumeMusic();
	Mix_Resume(-1);
}

void SoundManager::StopAll()
{
	Mix_HaltMusic();
	Mix_HaltChannel(-1);
}

void SoundManager::SetVolume(int p_volume, AudioFormat p_format)
{
	if (p_format == AudioFormat::WAV)
	{
		Mix_Volume(-1, p_volume);
	}
	else if (p_format == AudioFormat::MP3)
	{
		Mix_VolumeMusic(p_volume);
	}
}

void SoundManager::IncreaseVolume(int p_amount, AudioFormat p_format)
{
	if (p_format == AudioFormat::WAV)
	{
		int currentVolume = Mix_Volume(-1, -1);
		int newVolume = std::min(128, currentVolume + p_amount);
		Mix_Volume(-1, newVolume);
	}
	else if (p_format == AudioFormat::MP3)
	{
		int currentVolume = Mix_VolumeMusic(-1);
		int newVolume = std::min(128, currentVolume + p_amount);
		Mix_VolumeMusic(newVolume);
	}
}

void SoundManager::DecreaseVolume(int p_amount, AudioFormat p_format)
{
	if (p_format == AudioFormat::WAV)
	{
		int currentVolume = Mix_Volume(-1, -1);
		int newVolume = std::max(0, currentVolume - p_amount);
		Mix_Volume(-1, newVolume);
	}
	else if (p_format == AudioFormat::MP3)
	{
		int currentVolume = Mix_VolumeMusic(-1);
		int newVolume = std::max(0, currentVolume - p_amount);
		Mix_VolumeMusic(newVolume);
	}
}

#ifdef _DEBUG
void SoundManager::DebugVolume() const
{
	int sfxVolume = Mix_Volume(-1, -1);
	int musicVolume = Mix_VolumeMusic(-1);
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

void SoundManager::ApplySpatialization(Sound* p_sound)
{
	if (m_isSpatializationEnabled && p_sound != nullptr && p_sound->GetAudioFormat() == AudioFormat::WAV)
	{
		Vector2f listenerPos;
		if (m_listener == nullptr)
		{
			listenerPos = (Vector2f)Application::Get()->GetWindow()->GetWindowSize() / 2;
		}
		else
		{
			listenerPos = m_listener->GetPosition();
		}
		Vector2f soundPos = p_sound->GetPosition();

		float dx = soundPos.GetX() - listenerPos.GetX();
		float dy = soundPos.GetY() - listenerPos.GetY();

		float distanceSq = dx * dx + dy * dy;

		float angleDeg;
		//Car dans la documentation de SDL_mixer, les sons à une distance de 0 sont considérés comme étant directement devant le listener,
		//donc on peut mettre une petite marge pour éviter les problèmes de spatialisation lorsque le son est très proche du listener
		//(la distance 0 à 5 peut toutes produire le même effet, 6 à 10 sont égales, etc.)
		if (distanceSq < 25.0f)
		{
			angleDeg = 0.0f;
		}
		else
		{
			float angleRad = atan2f(dy, dx);
			angleDeg = (angleRad * 180.0f / M_PI) + 90;

			if (angleDeg < 0.0f)
				angleDeg += 360.0f;
		}
		if (distanceSq == p_sound->GetDistanceSquared() && angleDeg == p_sound->GetAngle())
		{
			return;
		}
		p_sound->SetDistanceSquared(distanceSq);
		float distance = std::min(255.0f, (sqrtf(distanceSq) / m_maxAudibleDistance) * 255.0f);
		p_sound->SetAngle(angleDeg);
		p_sound->SetDistance(distance);

		if (p_sound->GetChannel() != -1)
		{
			Mix_SetPosition(p_sound->GetChannel(), (Sint16)angleDeg, (Uint8)distance);
		}
	}
}

void SoundManager::SetMaxAudibleDistance(float p_distance)
{
	m_maxAudibleDistance = p_distance;
	if (m_maxAudibleDistance <= 0)
	{
		m_maxAudibleDistance = 1.0f;
	}
	m_ratioDistanceToVolume = 128.0f / m_maxAudibleDistance;
}

void SoundManager::SetRatioDistanceToVolume(float p_ratio)
{
	m_ratioDistanceToVolume = p_ratio;
	if (m_ratioDistanceToVolume <= 0)
	{
		m_ratioDistanceToVolume = 1.0f;
	}
	m_maxAudibleDistance = 128.0f / m_ratioDistanceToVolume;
}