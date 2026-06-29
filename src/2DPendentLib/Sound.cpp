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
	if (m_WAV) {
		Mix_FreeChunk(m_WAV);
		m_WAV = nullptr;
	}
	if (m_MUSIC) {
		Mix_FreeMusic(m_MUSIC);
		m_MUSIC = nullptr;
	}
}

void Sound::Initialize(const std::string& p_path, Vector2f p_position)
{
	m_audioFormat = DetectAudioFormat(p_path);

	if (m_audioFormat == AudioFormat::WAV) {
		m_WAV = Mix_LoadWAV(p_path.c_str());
		if (!m_WAV)
		{
			SDL_Log("Lib/Sound.cpp/Initialize/Mix_LoadWAV/ SFX not found : (%s): %s", p_path.c_str(), Mix_GetError());
		}
		SetPosition(p_position);
	}
	else if (m_audioFormat == AudioFormat::MP3) {
		m_MUSIC = Mix_LoadMUS(p_path.c_str());
		if (!m_MUSIC)
		{
			SDL_Log("Lib/Sound.cpp/Initialize/Mix_LoadMUS/ MUSIC not found : (%s): %s", p_path.c_str(), Mix_GetError());
		}
		SetPosition(p_position);
	}
}

AudioFormat Sound::DetectAudioFormat(const std::string& p_path)
{
	std::filesystem::path path(p_path);
	std::string ext = path.extension().string();
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
	if (ext == ".mp3" || ext == "ogg")
	{
		return AudioFormat::MP3;
	}
	if (ext == ".wav")
	{
		return AudioFormat::WAV;
	}
	return AudioFormat::NONE;
}

void Sound::Play(int p_loops, Sint16 p_angle, Uint8 p_distance)
{
	if(m_audioFormat == AudioFormat::WAV)
	{
		if ((m_lastChannel = Mix_PlayChannel(-1, m_WAV, p_loops)) == -1)
		{
			std::cerr << "Lib/Sound.cpp/Play/Mix_PlayChannel/Failed to play sound: " << Mix_GetError() << std::endl;
		}
		if (m_lastChannel != -1)
		{
			Mix_SetPosition(m_lastChannel, p_angle, p_distance); // (angle, distance) pour la spatialisation, ici on met une position fixe pour l'exemple
		}
	}
	else if (m_audioFormat == AudioFormat::MP3)
	{
		if(Mix_PlayMusic(m_MUSIC, p_loops) == -1)
		{
			std::cerr << "Lib/Sound.cpp/Play/Mix_PlayMusic/Failed to play music: " << Mix_GetError() << std::endl;
		}
	}
}

void Sound::Stop() {
	if (m_audioFormat == AudioFormat::WAV && m_lastChannel != -1)
	{
		Mix_HaltChannel(m_lastChannel);
		m_lastChannel = -1;
	}
	else if (m_audioFormat == AudioFormat::MP3)
	{
		Mix_HaltMusic();
	}
}

void Sound::Pause() const {
	if (m_audioFormat == AudioFormat::WAV && m_lastChannel != -1)
	{
		Mix_Pause(m_lastChannel);
	}
	else if (m_audioFormat == AudioFormat::MP3)
	{
		Mix_PauseMusic();
	}
}

void Sound::Resume() const{
	if (m_audioFormat == AudioFormat::WAV && m_lastChannel != -1)
	{
		Mix_Resume(m_lastChannel);
	}
	else if (m_audioFormat == AudioFormat::MP3)
	{
		Mix_ResumeMusic();
	}
}