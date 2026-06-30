#include "pch.h"
#include "Sound.h"
#include "Utils.h"

Sound::Sound(MIX_Mixer* p_mixer, Vector2i p_wSize)
{
	m_mixer = p_mixer;
	m_propertiesID = SDL_CreateProperties();
	if(m_propertiesID == 0)
	{
		SDL_Log("Lib/Sound.cpp/Sound/SDL_CreateProperties failed: %s", SDL_GetError());
		return;
	}
	float distanceDiagonale = std::hypot(p_wSize.GetX(), p_wSize.GetY());
	SetLimitDistanceToListen(100.0f, distanceDiagonale);

	// Param�tres du son
	m_loopCount = 0; // Nombre de boucles pour la lecture du son (-1 = boucle infinie, 0 = pas de boucle, NB > 0 = nombre de boucles)
	m_StartTimeMs = 0; // Point de d�part de la boucle en millisecondes pour la lecture du son
	m_LoopStartTimeMs = 0; // Point de d�part de la boucle en millisecondes pour la lecture du son
	m_maxTimeToPlayMs = -1; // Temps maximum de lecture du son en millisecondes. Si le son d�passe cette dur�e, il sera arr�t� automatiquement
	m_fadeInMs = 0; // Dur�e du fondu d'entr�e en millisecondes, utilis�e pour le fondu d'entr�e du son
	m_gainVolumeFadeIn = 1.0f; // Volume initial pour le fondu d'entr�e, utilis� pour le fondu d'entr�e du son
	m_fadeOutMs = 0; // Dur�e du fondu de sortie en millisecondes, utilis�e pour le fondu de sortie du son
	m_soundReverberation = 0; //Dur�e de la r�verb�ration du son en millisecondes, utilis�e pour la r�verb�ration du son

	m_isSpacializationEnabled = true; // Bool pour savoir si la spatialisation est activ�e ou non, utile pour la fonction Play() pour d�cider d'appliquer ou non les param�tres de spatialisation

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
		m_audio = MIX_LoadAudio(m_mixer, p_path.c_str(), true);
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
		// Si la piste ne joue plus (�puis�e), on la lib�re de la m�moire
		if (MIX_TrackPlaying(*it) == false)
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

void Sound::Play(const Vector2f& p_listenerPosition)
{
	if (m_audio == nullptr)
	{
		SDL_Log("Lib/Sound.cpp/Play/m_audio is nullptr");
		return;
	}

	if (m_audioFormat == AudioFormat::WAV)
	{
		MIX_Track* newTrack = MIX_CreateTrack(m_mixer);
		if (newTrack == nullptr)
		{
			SDL_Log("Lib/Sound.cpp/Play: MIX_CreateTrack failed: %s", SDL_GetError());
			return;
		}

		if (MIX_SetTrackAudio(newTrack, m_audio) == false)
		{
			SDL_Log("Lib/Sound.cpp/Play: MIX_SetTrackAudio failed: %s", SDL_GetError());
			MIX_DestroyTrack(newTrack);
			return;
		}

		UpdateRelativeVolume(m_lastGlobalVolume);

		if (m_isSpacializationEnabled)
		{
			UpdateRelativePosition(p_listenerPosition);

			MIX_Point3D position;
			float gain;
			ComputeSpatialization(position, gain);

			m_currentSmoothedGain = gain;
			m_currentSmoothedPanX = position.x;
			m_currentSmoothedPanZ = position.z;

			if (MIX_SetTrack3DPosition(newTrack, &position) == false)
			{
				SDL_Log("Lib/Sound.cpp/Play: MIX_SetTrack3DPosition failed: %s", SDL_GetError());
				return;
			}
			if (MIX_SetTrackGain(newTrack, gain) == false)
			{
				SDL_Log("Lib/Sound.cpp/Play: MIX_SetTrackGain failed: %s", SDL_GetError());
				MIX_DestroyTrack(newTrack);
				return;
			}
		}

		if (SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_LOOPS_NUMBER, m_loopCount) == false)
		{
			SDL_Log("Lib/Sound.cpp/Play: SDL_SetNumberProperty (MIX_PROP_PLAY_LOOPS_NUMBER) failed: %s", SDL_GetError());
			return;
		}
		if (SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_START_MILLISECOND_NUMBER, m_StartTimeMs) == false)
		{
			SDL_Log("Lib/Sound.cpp/Play: SDL_SetNumberProperty (MIX_PROP_PLAY_START_MILLISECOND_NUMBER) failed: %s", SDL_GetError());
			return;
		}
		if (SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_LOOP_START_MILLISECOND_NUMBER, m_LoopStartTimeMs) == false)
		{
			SDL_Log("Lib/Sound.cpp/Play: SDL_SetNumberProperty (MIX_PROP_PLAY_LOOP_START_MILLISECOND_NUMBER) failed: %s", SDL_GetError());
			return;
		}
		if (SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_MAX_MILLISECONDS_NUMBER, m_maxTimeToPlayMs) == false)
		{
			SDL_Log("Lib/Sound.cpp/Play: SDL_SetNumberProperty (MIX_PROP_PLAY_MAX_MILLISECONDS_NUMBER) failed: %s", SDL_GetError());
			return;
		}
		if (SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, m_fadeInMs) == false)
		{
			SDL_Log("Lib/Sound.cpp/Play: SDL_SetNumberProperty (MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER) failed: %s", SDL_GetError());
			return;
		}
		if (SDL_SetFloatProperty(m_propertiesID, MIX_PROP_PLAY_FADE_IN_START_GAIN_FLOAT, m_gainVolumeFadeIn) == false)
		{
			SDL_Log("Lib/Sound.cpp/Play: SDL_SetFloatProperty (MIX_PROP_PLAY_FADE_IN_START_GAIN_FLOAT) failed: %s", SDL_GetError());
			return;
		}
		if (SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_APPEND_SILENCE_MILLISECONDS_NUMBER, m_soundReverberation) == false)
		{
			SDL_Log("Lib/Sound.cpp/Play: SDL_SetNumberProperty (MIX_PROP_PLAY_APPEND_SILENCE_MILLISECONDS_NUMBER) failed: %s", SDL_GetError());
			return;
		}

		if (MIX_PlayTrack(newTrack, m_propertiesID) == false)
		{
			SDL_Log("Lib/Sound.cpp/Play: MIX_PlayTrack failed: %s", SDL_GetError());
			return;
		}

		m_pistesSFX.push_back(newTrack);
	}

	else if (m_audioFormat == AudioFormat::MP3)
	{
		if (!m_pisteMusique)
		{
			m_pisteMusique = MIX_CreateTrack(m_mixer);
			if (m_pisteMusique == nullptr)
			{
				SDL_Log("Lib/Sound.cpp/Play: MIX_CreateTrack failed: %s", SDL_GetError());
				return;
			}
			if (MIX_SetTrackAudio(m_pisteMusique, m_audio) == false)
			{
				SDL_Log("Lib/Sound.cpp/Play: MIX_SetTrackAudio failed: %s", SDL_GetError());
				return;
			}
		}

		UpdateRelativeVolume(m_lastGlobalVolume);

		if (m_isSpacializationEnabled)
		{
			UpdateRelativePosition(p_listenerPosition);

			MIX_Point3D position;
			float gain;
			ComputeSpatialization(position, gain);

			m_currentSmoothedGain = gain;
			m_currentSmoothedPanX = position.x;
			m_currentSmoothedPanZ = position.z;

			if (MIX_SetTrack3DPosition(m_pisteMusique, &position) == false)
			{
				SDL_Log("Lib/Sound.cpp/Play: MIX_SetTrack3DPosition failed: %s", SDL_GetError());
				return;
			}
			if (MIX_SetTrackGain(m_pisteMusique, gain) == false)
			{
				SDL_Log("Lib/Sound.cpp/Play: MIX_SetTrackGain failed: %s", SDL_GetError());
				return;
			}
		}

		if (SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_LOOPS_NUMBER, m_loopCount) == false)
		{
			SDL_Log("Lib/Sound.cpp/Play: SDL_SetNumberProperty (MIX_PROP_PLAY_LOOPS_NUMBER) failed: %s", SDL_GetError());
			return;
		}
		if (SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_START_MILLISECOND_NUMBER, m_StartTimeMs) == false)
		{
			SDL_Log("Lib/Sound.cpp/Play: SDL_SetNumberProperty (MIX_PROP_PLAY_START_MILLISECOND_NUMBER) failed: %s", SDL_GetError());
			return;
		}
		if (SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_LOOP_START_MILLISECOND_NUMBER, m_LoopStartTimeMs) == false)
		{
			SDL_Log("Lib/Sound.cpp/Play: SDL_SetNumberProperty (MIX_PROP_PLAY_LOOP_START_MILLISECOND_NUMBER) failed: %s", SDL_GetError());
			return;
		}
		if (SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_MAX_MILLISECONDS_NUMBER, m_maxTimeToPlayMs) == false)
		{
			SDL_Log("Lib/Sound.cpp/Play: SDL_SetNumberProperty (MIX_PROP_PLAY_MAX_MILLISECONDS_NUMBER) failed: %s", SDL_GetError());
			return;
		}
		if (SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, m_fadeInMs) == false)
		{
			SDL_Log("Lib/Sound.cpp/Play: SDL_SetNumberProperty (MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER) failed: %s", SDL_GetError());
			return;
		}
		if (SDL_SetFloatProperty(m_propertiesID, MIX_PROP_PLAY_FADE_IN_START_GAIN_FLOAT, m_gainVolumeFadeIn) == false)
		{
			SDL_Log("Lib/Sound.cpp/Play: SDL_SetFloatProperty (MIX_PROP_PLAY_FADE_IN_START_GAIN_FLOAT) failed: %s", SDL_GetError());
			return;
		}
		if (SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_APPEND_SILENCE_MILLISECONDS_NUMBER, m_soundReverberation) == false)
		{
			SDL_Log("Lib/Sound.cpp/Play: SDL_SetNumberProperty (MIX_PROP_PLAY_APPEND_SILENCE_MILLISECONDS_NUMBER) failed: %s", SDL_GetError());
			return;
		}

		if (MIX_PlayTrack(m_pisteMusique, m_propertiesID) == false)
		{
			SDL_Log("Lib/Sound.cpp/Play: MIX_PlayTrack failed: %s", SDL_GetError());
			return;
		}
	}
}

void Sound::Stop()
{

	if (m_audioFormat == AudioFormat::MP3 && m_pisteMusique)
	{
		Sint64 MsInFrame = MIX_TrackMSToFrames(m_pisteMusique, m_fadeOutMs);
		if (MsInFrame < 0)
		{
			SDL_Log("Lib/Sound.cpp/Stop: MIX_TrackMSToFrames failed: %s", SDL_GetError());
			return;
		}
		if (MIX_StopTrack(m_pisteMusique, MsInFrame) == false)
		{
			SDL_Log("Lib/Sound.cpp/Stop: MIX_StopTrack failed: %s", SDL_GetError());
			return;
		}
	}
	else if (m_audioFormat == AudioFormat::WAV)
	{
		Sint64 MsInFrame;
		for (MIX_Track* piste : m_pistesSFX)
		{
			MsInFrame = MIX_TrackMSToFrames(piste, m_fadeOutMs);
			if (MsInFrame < 0)
			{
				SDL_Log("Lib/Sound.cpp/Stop: MIX_TrackMSToFrames failed: %s", SDL_GetError());
				return;
			}
			if (MIX_StopTrack(piste, MsInFrame) == false)
			{
				SDL_Log("Lib/Sound.cpp/Stop: MIX_StopTrack failed: %s", SDL_GetError());
				return;
			}
		}
	}
}

void Sound::Pause() const
{
	if (m_audioFormat == AudioFormat::MP3 && m_pisteMusique)
	{
		if (MIX_PauseTrack(m_pisteMusique) == false)
		{
			SDL_Log("Lib/Sound.cpp/Pause: MIX_PauseTrack failed: %s", SDL_GetError());
			return;
		}
	}
	else if (m_audioFormat == AudioFormat::WAV)
	{
		for (MIX_Track* piste : m_pistesSFX)
		{
			if (MIX_PauseTrack(piste) == false)
			{
				SDL_Log("Lib/Sound.cpp/Pause: MIX_PauseTrack failed: %s", SDL_GetError());
				return;
			}
			
		}
	}
}

void Sound::Resume() const
{
	if (m_audioFormat == AudioFormat::MP3 && m_pisteMusique)
	{
		if (MIX_ResumeTrack(m_pisteMusique) == false)
		{
			SDL_Log("Lib/Sound.cpp/Resume: MIX_ResumeTrack failed: %s", SDL_GetError());
			return;
		}
	}
	else if (m_audioFormat == AudioFormat::WAV)
	{
		for (MIX_Track* piste : m_pistesSFX)
		{
			if (MIX_ResumeTrack(piste) == false)
			{
				SDL_Log("Lib/Sound.cpp/Resume: MIX_ResumeTrack failed: %s", SDL_GetError());
				return;
			}
		}
	}
}

// D�finit le nombre de boucles pour la lecture du son (-1 = boucle infinie, 0 = pas de boucle, NB > 0 = nombre de boucles).
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

// D�finit le point de d�part de la boucle en secondes pour la lecture du son.
void Sound::SetLoopStartAtTime(int p_startTime)
{
	if (p_startTime == 0)
	{
		m_LoopStartTimeMs = 0;
		return;
	}
	m_LoopStartTimeMs = static_cast<Sint64>(p_startTime * 1000.0);
}

// D�finit le temps maximum de lecture du son en secondes. Si le son d�passe cette dur�e, il sera arr�t� automatiquement.
void Sound::SetMaxTimeToPlay(int p_maxTime)
{
	if (p_maxTime == -1)
	{
		m_maxTimeToPlayMs = -1;
		return;
	}

	m_maxTimeToPlayMs = static_cast<Sint64>(p_maxTime * 1000.0);
}

// D�finit le volume initial et la dur�e du fondu d'entr�e en millisecondes.
void Sound::SetVolumeFadeIn(int p_seconde, int p_volume)
{
	Utils::LimitMin(p_seconde, 0);
	if (p_seconde == 0 && p_volume == 0)
	{
		m_fadeInMs = 0;
		m_gainVolumeFadeIn = 0;
		return;
	}

	m_fadeInMs = static_cast<Sint64>(p_seconde * 1000);

	float gain = static_cast<float>(p_volume) / 100.0f;
	Utils::Clamp(gain, 0.0f, 1.0f);

	m_gainVolumeFadeIn = gain;
}

// D�finit le volume initial et la dur�e du fondu de sortie en secondes.
void Sound::SetVolumeFadeOut(int p_seconde)
{
	int fadeout = Utils::LimitMin(p_seconde, 0);

	m_fadeOutMs = static_cast<Sint64>(fadeout * 1000);
}

//Dur�e de la r�verb�ration du son en seconde, utilis�e pour la r�verb�ration du son
void Sound::SetSoundReverberation(int p_seconde)
{
	// S�curit� pour rester au dessus de 0
	int reverb = Utils::LimitMin(p_seconde, 0);

	m_soundReverberation = static_cast<Sint64>(reverb * 1000);
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

void Sound::SetLimitDistanceToListen(float p_min, float p_max)
{
	m_minDistanceToListen = Utils::LimitMin(p_min, 0.0f);
	m_maxDistanceToListen = Utils::LimitMin(p_max, 0.0f);
}

void Sound::ApplySpatialization(const Vector2f& p_listenerPosition, float p_deltaTime)
{
	if (m_isSpacializationEnabled == false)
	{
		return;
	}

	UpdateRelativePosition(p_listenerPosition);

	if (!m_pistesSFX.empty() || m_pisteMusique != nullptr)
	{
		MIX_Point3D targetPosition;
		float targetGain;

		// 1. On r�cup�re les valeurs CIBLES (math�matiquement parfaites)
		ComputeSpatialization(targetPosition, targetGain);

		// 2. Calcul de la vitesse d'interpolation (Slew Rate)
		// 15.0f est une bonne valeur de base : assez rapide pour suivre l'action, 
		// mais assez doux pour �viter les "clics" audio.
		float interpSpeed = 15.0f * p_deltaTime;
		Utils::Clamp(interpSpeed, 0.0f, 1.0f); // S�curit� anti-lag

		// 3. Interpolation Lin�aire (Lerp) depuis la valeur actuelle vers la cible
		m_currentSmoothedGain = std::lerp(m_currentSmoothedGain, targetGain, interpSpeed);
		m_currentSmoothedPanX = std::lerp(m_currentSmoothedPanX, targetPosition.x, interpSpeed);
		m_currentSmoothedPanZ = std::lerp(m_currentSmoothedPanZ, targetPosition.z, interpSpeed);

		// Cr�ation du vecteur de position liss�
		MIX_Point3D finalPosition = { m_currentSmoothedPanX, 0.0f, m_currentSmoothedPanZ };

		if (m_audioFormat == AudioFormat::MP3 && m_pisteMusique)
		{
			MIX_SetTrack3DPosition(m_pisteMusique, &finalPosition);
			MIX_SetTrackGain(m_pisteMusique, m_currentSmoothedGain);
		}
		else if (m_audioFormat == AudioFormat::WAV && !m_pistesSFX.empty())
		{
			// 4. On applique les valeurs LISS�ES aux pistes
			for (MIX_Track* piste : m_pistesSFX)
			{
				MIX_SetTrack3DPosition(piste, &finalPosition);
				MIX_SetTrackGain(piste, m_currentSmoothedGain);
			}
		}
	}
}

float Sound::ComputeDistanceGain(float p_distance) const
{
	if (p_distance <= m_minDistanceToListen) return 1.0f;
	if (p_distance >= m_maxDistanceToListen) return 0.0f; // vrai silence garanti

	float t = (p_distance - m_minDistanceToListen) / (m_maxDistanceToListen - m_minDistanceToListen);
	return 1.0f - t; // lin�aire ; remplace par (1.0f - t) * (1.0f - t) pour une chute plus douce au d�but
}

void Sound::UpdateRelativePosition(const Vector2f& p_listenerPosition)
{
	float dx = m_position.GetX() - p_listenerPosition.GetX();
	float dy = m_position.GetY() - p_listenerPosition.GetY();

	m_relativePosition.SetX(dx);
	m_relativePosition.SetY(dy);
}

void Sound::ComputeSpatialization(MIX_Point3D& p_outPosition, float& p_outGain) const
{
	float dx = m_relativePosition.GetX();
	float dy = m_relativePosition.GetY();
	float distanceSquared = dx * dx + dy * dy;
	float maxSq = m_maxDistanceToListen * m_maxDistanceToListen;

	if (distanceSquared >= maxSq)
	{
		p_outPosition.x = 0.0f;
		p_outPosition.y = 0.0f;
		p_outPosition.z = 0.0f;
		p_outGain = 0.0f;
		return;
	}

	float distance = std::sqrtf(distanceSquared);

	// Calcul de la position 3D pour le panoramique
	if (distance > 0.0001f)
	{
		// 1. Calcul du panoramique pur (direction)
		float panX = dx / distance;
		float panZ = dy / distance;

		// 2. Lissage si on est tr�s proche de la source
		if (distance < m_minDistanceToListen)
		{
			// Cr�e un ratio de 0.0 (au centre exact) � 1.0 (sur la bordure de la distance min)
			float x = distance / m_minDistanceToListen;

			// Optionnel : Mettre au carr� pour rendre la transition vers le centre encore plus douce
			float centerBlend = (x * x) * (3.0f - 2.0f * x);

			// Applique le ratio : plus on est proche, plus le son est forc� au centre (0.0)
			panX *= centerBlend;
			panZ *= centerBlend;
		}

		p_outPosition.x = panX;
		p_outPosition.z = panZ;
	}
	else
	{
		// Exactement sur la source = son parfaitement centr� (omnidirectionnel)
		p_outPosition.x = 0.0f;
		p_outPosition.z = 0.0f;
	}
	p_outPosition.y = 0.0f;

	float t = 0.0f;

	if (distance > m_minDistanceToListen)
	{
		// Ratio de 0.0 (� la distance min) jusqu'� 1.0 (� la distance max)
		t = (distance - m_minDistanceToListen) / (m_maxDistanceToListen - m_minDistanceToListen);
	}

	Utils::Clamp(t, 0.0f, 1.0f);

	// Inverse le ratio (1 = proche, 0 = loin)
	float invertedRatio = 1.0f - t;

	// On met au carr� pour cr�er une courbe qui s'estompe doucement � la fin
	float naturalAttenuation = invertedRatio * invertedRatio;

	p_outGain = m_relativeVolume * naturalAttenuation;
}
