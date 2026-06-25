#include "pch.h"
#include "Sound.h"
#include "Utils.h"

Sound::Sound(MIX_Mixer* p_mixer, Vector2i p_wSize)
{
	m_mixer = p_mixer;
	m_propertiesID = SDL_CreateProperties();
	float distanceDiagonale = std::hypot(p_wSize.GetX(), p_wSize.GetY());
	SetLimitDistanceToListen(0.0f, distanceDiagonale / 2, 100);

}

Sound::~Sound() {
	if (m_pisteMusique)
	{
		MIX_DestroyTrack(m_pisteMusique);
		m_pisteMusique = nullptr;
	}

	for (auto& track : m_pistesSFX)
	{
		if (track)
		{
			MIX_DestroyTrack(track);
		}
	}
	m_pistesSFX.clear();

	if (m_audio)
	{
		MIX_DestroyAudio(m_audio);
		m_audio = nullptr;
	}

	if (m_propertiesID)
	{
		SDL_DestroyProperties(m_propertiesID);
	}
}

void Sound::Initialize(const std::string& p_path, Vector2f p_position)
{
	m_audioFormat = DetectAudioFormat(p_path);

	//MIX_CreateTrack()

	if (m_audioFormat == AudioFormat::WAV)
	{
		m_audio = MIX_LoadAudio(m_mixer,p_path.c_str(), true);
		if (!m_audio)
		{
			SDL_Log("Lib/Sound.cpp/Initialize/Mix_LoadWAV/ SFX not found : (%s): %s", p_path.c_str(), SDL_GetError());
			return;
		}
	}
	else if (m_audioFormat == AudioFormat::MP3)
	{
		m_audio = MIX_LoadAudio(m_mixer, p_path.c_str(), false);
		if (!m_audio)
		{
			SDL_Log("Lib/Sound.cpp/Initialize/Mix_LoadMUS/ MUSIC not found : (%s): %s", p_path.c_str(), SDL_GetError());
			return;
		}
	}
	SetPosition(p_position);
}

void Sound::RemoveOldTrack()
{
	for (auto it = m_pistesSFX.begin(); it != m_pistesSFX.end(); )
	{
		// Si la piste ne joue plus (épuisée), on la libère de la mémoire
		if (!MIX_TrackPlaying(*it))
		{
			MIX_DestroyTrack(*it);
			it = m_pistesSFX.erase(it); // On la retire du vecteur
		}
		else {
			++it;
		}
	}
}

AudioFormat Sound::DetectAudioFormat(const std::string& p_path)
{
	std::filesystem::path path(p_path);
	std::string ext = path.extension().string();
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
	if (ext == ".mp3")
	{
		return AudioFormat::MP3;
	}
	if (ext == ".wav")
	{
		return AudioFormat::WAV;
	}
	return AudioFormat::NONE;
}

void Sound::Play()
{
	if (m_audio == nullptr)
	{
		SDL_Log("Lib/Sound.cpp/Play/m_audio is nullptr");
		return;
	}

	if(m_audioFormat == AudioFormat::WAV)
	{
		MIX_Track* newTrack = MIX_CreateTrack(m_mixer);
		if (newTrack == nullptr)
		{
			SDL_Log("Lib/Sound.cpp/Play: MIX_CreateTrack failed: %s", SDL_GetError());
			return;
		}

		MIX_SetTrackAudio(newTrack, m_audio);

		MIX_SetTrackGain(newTrack, m_relativeVolume);

		MIX_Point3D position;
		// Axe X (Gauche/Droite) = dx de l'écran
		position.x = m_relativePosition.GetX() / m_rolloffDistanceToVolume;

		// Vu que je peux pas encore tester si il fonctionne le code si bug de spacialisation inverser le GetY et le 0.0f de .z

		// Axe Y (Haut/Bas dans l'espace)
		position.z = m_relativePosition.GetY() / m_rolloffDistanceToVolume;

		// Axe Z (Profondeur Devant/Derrière)
		position.y = 0.0f;
		MIX_SetTrack3DPosition(newTrack, &position);

		SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_LOOPS_NUMBER, m_loopCount);
		SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_START_MILLISECOND_NUMBER, m_StartTimeMs);
		SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_LOOP_START_MILLISECOND_NUMBER, m_LoopStartTimeMs);
		SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_MAX_MILLISECONDS_NUMBER, m_maxTimeToPlayMs);
		SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, m_fadeInMs);
		SDL_SetFloatProperty(m_propertiesID, MIX_PROP_PLAY_FADE_IN_START_GAIN_FLOAT, m_gainVolumeFadeIn);
		SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_APPEND_SILENCE_MILLISECONDS_NUMBER, m_soundReverberation);

		MIX_PlayTrack(newTrack, m_propertiesID);

		m_pistesSFX.push_back(newTrack);
	}

	else if (m_audioFormat == AudioFormat::MP3)
	{
		if (!m_pisteMusique)
		{
			m_pisteMusique = MIX_CreateTrack(m_mixer);
			MIX_SetTrackAudio(m_pisteMusique, m_audio);
		}

		MIX_SetTrackGain(m_pisteMusique, m_relativeVolume);

		SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_LOOPS_NUMBER, m_loopCount);
		SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_START_MILLISECOND_NUMBER, m_StartTimeMs);
		SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_LOOP_START_MILLISECOND_NUMBER, m_LoopStartTimeMs);
		SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_MAX_MILLISECONDS_NUMBER, m_maxTimeToPlayMs);
		SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, m_fadeInMs);
		SDL_SetFloatProperty(m_propertiesID, MIX_PROP_PLAY_FADE_IN_START_GAIN_FLOAT, m_gainVolumeFadeIn);
		SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_APPEND_SILENCE_MILLISECONDS_NUMBER, m_soundReverberation);

		MIX_SetTrack3DPosition(m_pisteMusique, nullptr);
		MIX_PlayTrack(m_pisteMusique, m_propertiesID);
	}
}

void Sound::Stop()
{

	if (m_audioFormat == AudioFormat::MP3 && m_pisteMusique)
	{
		MIX_StopTrack(m_pisteMusique, m_fadeOutMs);
	}
	else if (m_audioFormat == AudioFormat::WAV)
	{
		for (MIX_Track* piste : m_pistesSFX)
		{
			MIX_StopTrack(piste, m_fadeOutMs);
		}
	}
}

void Sound::Pause() const
{
	if (m_audioFormat == AudioFormat::MP3 && m_pisteMusique)
	{
		MIX_PauseTrack(m_pisteMusique);
	}
	else if (m_audioFormat == AudioFormat::WAV)
	{
		for (MIX_Track* piste : m_pistesSFX)
		{
			MIX_PauseTrack(piste);
		}
	}
}

void Sound::Resume() const
{
	if (m_audioFormat == AudioFormat::MP3 && m_pisteMusique)
	{
		MIX_ResumeTrack(m_pisteMusique);
	}
	else if (m_audioFormat == AudioFormat::WAV)
	{
		for (MIX_Track* piste : m_pistesSFX)
		{
			MIX_ResumeTrack(piste);
		}
	}
}

// Définit le nombre de boucles pour la lecture du son (-1 = boucle infinie, 0 = pas de boucle, NB > 0 = nombre de boucles).
void Sound::SetLoopCount(int p_loopCount)
{
	m_loopCount = p_loopCount;
}

void Sound::SetStartAtTime(int p_startTime)
{
	if (p_startTime == 0)
	{
		m_StartTimeMs = 0;
		return;
	}
	m_StartTimeMs = static_cast<Sint64>(p_startTime * 1000.0);

}

// Définit le point de départ de la boucle en secondes pour la lecture du son.
void Sound::SetLoopStartAtTime(int p_startTime)
{
	if (p_startTime == 0)
	{
		m_LoopStartTimeMs = 0;
		return;
	}
	m_LoopStartTimeMs = static_cast<Sint64>(p_startTime * 1000.0);
}

// Définit le temps maximum de lecture du son en secondes. Si le son dépasse cette durée, il sera arrêté automatiquement.
void Sound::SetMaxTimeToPlay(int p_maxTime)
{
	if (p_maxTime == -1)
	{
		m_maxTimeToPlayMs = -1;
		return;
	}

	m_maxTimeToPlayMs = static_cast<Sint64>(p_maxTime * 1000.0);
}

// Définit le volume initial et la durée du fondu d'entrée en millisecondes.
void Sound::SetVolumeFadeIn(int p_ms, int p_volume)
{
	Utils::LimitMin(p_ms, 0);
	if (p_ms == 0 && p_volume == 0)
	{
		m_fadeInMs = 0;
		m_gainVolumeFadeIn = 0;
		return;
	}

	m_fadeInMs = p_ms;
	
	float gain = static_cast<float>(p_volume) / 100.0f;
	Utils::Clamp(gain, 0.0f, 1.0f);

	m_gainVolumeFadeIn = gain;
}

// Définit le volume initial et la durée du fondu de sortie en millisecondes.
void Sound::SetVolumeFadeOut(int p_ms)
{
	m_fadeOutMs = Utils::LimitMin(p_ms, 0);
}

// Définit la réverbération du son en fonction d'un facteur de réverbération (p_reverb) compris entre 0.0f (pas de réverbération) et 1.0f (réverbération maximale).
void Sound::SetSoundReverberation(float p_reverb)
{
	// Sécurité pour rester entre 0.0f et 1.0f
	Utils::Clamp(p_reverb, 0.0f, 1.0f);

	m_soundReverberation = static_cast<Sint64>(p_reverb * m_maxReverberation);
}

void Sound::SetAbsoluteVolume(int p_volume)
{
	float gain = static_cast<float>(p_volume) / 100.0f;
	m_absoluteVolume = Utils::Clamp(gain, 0.0f, 1.0f);

	UpdateRelativeVolume(m_lastGlobalVolume);
}

void Sound::UpdateRelativeVolume(float p_globalVolume)
{
	m_lastGlobalVolume = Utils::Clamp(p_globalVolume, 0.0f, 1.0f);

	m_relativeVolume = m_absoluteVolume * m_lastGlobalVolume;

	if (m_audioFormat == AudioFormat::MP3 && m_pisteMusique)
	{
		MIX_SetTrackGain(m_pisteMusique, m_relativeVolume);
	}
	else if (m_audioFormat == AudioFormat::WAV)
	{
		for (MIX_Track* piste : m_pistesSFX)
		{
			MIX_SetTrackGain(piste, m_relativeVolume);
		}
	}
}

void Sound::SetLimitDistanceToListen(float p_min, float p_max, float p_rolloff)
{
	m_minDistanceToListen = Utils::LimitMin(p_min, 0.0f);
	m_maxDistanceToListen = Utils::LimitMin(p_max, 0.0f);
	m_rolloffDistanceToVolume = Utils::LimitMin(p_rolloff, 1.f);
}

void Sound::ApplySpatialization(const Vector2f& p_listenerPosition)
{
	float dx = m_position.GetX() - p_listenerPosition.GetX();
	float dy = m_position.GetY() - p_listenerPosition.GetY();

	m_relativePosition.SetX(dx);
	m_relativePosition.SetY(dy);

	MIX_Point3D position;
	position.x = dx / m_rolloffDistanceToVolume;
	position.z = dy / m_rolloffDistanceToVolume;
	position.y = 0.0f;

	if (m_audioFormat == AudioFormat::WAV && !m_pistesSFX.empty())
	{
		for (MIX_Track* piste : m_pistesSFX)
		{
			MIX_SetTrack3DPosition(piste, &position);
		}
	}
	/*else if (m_audioFormat == AudioFormat::MP3 && m_pisteMusique != nullptr)
	{
		MIX_SetTrack3DPosition(m_pisteMusique, &position);
	}*/
}
