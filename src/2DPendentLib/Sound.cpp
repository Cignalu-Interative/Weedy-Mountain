#include "pch.h"
#include "Sound.h"
#include "Utils.h"

Sound::Sound(MIX_Mixer* p_mixer, Vector2i p_wSize)
{
	m_mixer = p_mixer;
	m_propertiesID = SDL_CreateProperties();
	float distanceDiagonale = std::hypot(p_wSize.GetX(), p_wSize.GetY());
	SetLimitDistanceToListen(100.0f, distanceDiagonale);

	// Paramètres du son
	m_loopCount = 0; // Nombre de boucles pour la lecture du son (-1 = boucle infinie, 0 = pas de boucle, NB > 0 = nombre de boucles)
	m_StartTimeMs = 0; // Point de départ de la boucle en millisecondes pour la lecture du son
	m_LoopStartTimeMs = 0; // Point de départ de la boucle en millisecondes pour la lecture du son
	m_maxTimeToPlayMs = -1; // Temps maximum de lecture du son en millisecondes. Si le son dépasse cette durée, il sera arrêté automatiquement
	m_fadeInMs = 0; // Durée du fondu d'entrée en millisecondes, utilisée pour le fondu d'entrée du son
	m_gainVolumeFadeIn = 1.0f; // Volume initial pour le fondu d'entrée, utilisé pour le fondu d'entrée du son
	m_fadeOutMs = 0; // Durée du fondu de sortie en millisecondes, utilisée pour le fondu de sortie du son
	m_soundReverberation = 0; //Durée de la réverbération du son en millisecondes, utilisée pour la réverbération du son

	m_isSpacializationEnabled = true; // Bool pour savoir si la spatialisation est activée ou non, utile pour la fonction Play() pour décider d'appliquer ou non les paramètres de spatialisation

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

void Sound::Play(const Vector2f& p_listenerPosition)
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

			MIX_SetTrack3DPosition(newTrack, &position);
			MIX_SetTrackGain(newTrack, gain);
		}

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

			MIX_SetTrack3DPosition(m_pisteMusique, &position);
			MIX_SetTrackGain(m_pisteMusique, gain);
		}

		SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_LOOPS_NUMBER, m_loopCount);
		SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_START_MILLISECOND_NUMBER, m_StartTimeMs);
		SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_LOOP_START_MILLISECOND_NUMBER, m_LoopStartTimeMs);
		SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_MAX_MILLISECONDS_NUMBER, m_maxTimeToPlayMs);
		SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, m_fadeInMs);
		SDL_SetFloatProperty(m_propertiesID, MIX_PROP_PLAY_FADE_IN_START_GAIN_FLOAT, m_gainVolumeFadeIn);
		SDL_SetNumberProperty(m_propertiesID, MIX_PROP_PLAY_APPEND_SILENCE_MILLISECONDS_NUMBER, m_soundReverberation);

		MIX_PlayTrack(m_pisteMusique, m_propertiesID);
	}
}

void Sound::Stop()
{

	if (m_audioFormat == AudioFormat::MP3 && m_pisteMusique)
	{
		Sint64 MsInFrame = MIX_TrackMSToFrames(m_pisteMusique, m_fadeOutMs);
		MIX_StopTrack(m_pisteMusique, MsInFrame);
	}
	else if (m_audioFormat == AudioFormat::WAV)
	{
		Sint64 MsInFrame;
		for (MIX_Track* piste : m_pistesSFX)
		{
			MsInFrame = MIX_TrackMSToFrames(piste, m_fadeOutMs);
			MIX_StopTrack(piste, MsInFrame);
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

// Définit le volume initial et la durée du fondu de sortie en secondes.
void Sound::SetVolumeFadeOut(int p_seconde)
{
	int fadeout = Utils::LimitMin(p_seconde, 0);

	m_fadeOutMs = static_cast<Sint64>(fadeout * 1000);
}

//Durée de la réverbération du son en seconde, utilisée pour la réverbération du son
void Sound::SetSoundReverberation(int p_seconde)
{
	// Sécurité pour rester au dessus de 0
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

		// 1. On récupère les valeurs CIBLES (mathématiquement parfaites)
		ComputeSpatialization(targetPosition, targetGain);

		// 2. Calcul de la vitesse d'interpolation (Slew Rate)
		// 15.0f est une bonne valeur de base : assez rapide pour suivre l'action, 
		// mais assez doux pour éviter les "clics" audio.
		float interpSpeed = 15.0f * p_deltaTime;
		Utils::Clamp(interpSpeed, 0.0f, 1.0f); // Sécurité anti-lag

		// 3. Interpolation Linéaire (Lerp) depuis la valeur actuelle vers la cible
		m_currentSmoothedGain = std::lerp(m_currentSmoothedGain, targetGain, interpSpeed);
		m_currentSmoothedPanX = std::lerp(m_currentSmoothedPanX, targetPosition.x, interpSpeed);
		m_currentSmoothedPanZ = std::lerp(m_currentSmoothedPanZ, targetPosition.z, interpSpeed);

		// Création du vecteur de position lissé
		MIX_Point3D finalPosition = { m_currentSmoothedPanX, 0.0f, m_currentSmoothedPanZ };

		if (m_audioFormat == AudioFormat::MP3 && m_pisteMusique)
		{
			MIX_SetTrack3DPosition(m_pisteMusique, &finalPosition);
			MIX_SetTrackGain(m_pisteMusique, m_currentSmoothedGain);
		}
		else if (m_audioFormat == AudioFormat::WAV && !m_pistesSFX.empty())
		{
			// 4. On applique les valeurs LISSÉES aux pistes
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
	return 1.0f - t; // linéaire ; remplace par (1.0f - t) * (1.0f - t) pour une chute plus douce au début
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

		// 2. Lissage si on est très proche de la source
		if (distance < m_minDistanceToListen)
		{
			// Crée un ratio de 0.0 (au centre exact) à 1.0 (sur la bordure de la distance min)
			float x = distance / m_minDistanceToListen;

			// Optionnel : Mettre au carré pour rendre la transition vers le centre encore plus douce
			float centerBlend = (x * x) * (3.0f - 2.0f * x);

			// Applique le ratio : plus on est proche, plus le son est forcé au centre (0.0)
			panX *= centerBlend;
			panZ *= centerBlend;
		}

		p_outPosition.x = panX;
		p_outPosition.z = panZ;
	}
	else
	{
		// Exactement sur la source = son parfaitement centré (omnidirectionnel)
		p_outPosition.x = 0.0f;
		p_outPosition.z = 0.0f;
	}
	p_outPosition.y = 0.0f;

	float t = 0.0f;

	if (distance > m_minDistanceToListen)
	{
		// Ratio de 0.0 (à la distance min) jusqu'à 1.0 (à la distance max)
		t = (distance - m_minDistanceToListen) / (m_maxDistanceToListen - m_minDistanceToListen);
	}

	Utils::Clamp(t, 0.0f, 1.0f);

	// Inverse le ratio (1 = proche, 0 = loin)
	float invertedRatio = 1.0f - t;

	// On met au carré pour créer une courbe qui s'estompe doucement à la fin
	float naturalAttenuation = invertedRatio * invertedRatio;

	p_outGain = m_relativeVolume * naturalAttenuation;
}
