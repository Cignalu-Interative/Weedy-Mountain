#pragma once
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <map>
#include <vector>
#include "Vector2.h"
#include "AudioFormat.h"
#include "Transformable.h"

class Sound : public Transformable
{
private:

	std::string m_name; //Nom du son, extrait du nom du fichier audio, utilisé comme identifiant dans SoundManager

	MIX_Mixer* m_mixer = nullptr; //Pointeur vers le mixer, utilisé pour gérer les sons et la musique

	MIX_Audio* m_audio = nullptr; //Pointeur vers l'audio, peut être un Mix_Chunk (pour les SFX) ou un Mix_Music (pour les MUSIC) en sdl2

	MIX_Track* m_pisteMusique = nullptr; // Pointeur vers la piste audio, !! Seulement stocker les MP3 car Sinon écrase la lecture en cours si spam
	std::vector<MIX_Track*> m_pistesSFX; // Pointeur vers la piste audio, !! Seulement stocker les SFX car Sinon écrase la lecture en cours si spam

	MIX_Point3D m_position3D; //Position 3D du son, utilisé pour la spatialisation (calculé à partir de la position du son et du listener)

	SDL_PropertiesID m_propertiesID = 0; //Identifiant des propriétés du son, utilisé pour la spatialisation (calculé à partir de la position du son et du listener)

	//Format de l'audio par default à NONE, il sera défini lors de l'initialisation en fonction du format du fichier audio
	AudioFormat m_audioFormat = AudioFormat::NONE;

	// Paramètres du son
	int m_loopCount = 0; // Nombre de boucles pour la lecture du son (-1 = boucle infinie, 0 = pas de boucle, NB > 0 = nombre de boucles)
	Sint64 m_StartTimeMs = 0; // Point de départ de la boucle en millisecondes pour la lecture du son
	Sint64 m_LoopStartTimeMs = 0; // Point de départ de la boucle en millisecondes pour la lecture du son
	int m_maxTimeToPlayMs = -1; // Temps maximum de lecture du son en millisecondes. Si le son dépasse cette durée, il sera arrêté automatiquement
	int m_fadeInMs = 0; // Durée du fondu d'entrée en millisecondes, utilisée pour le fondu d'entrée du son
	float m_gainVolumeFadeIn = 1.0f; // Volume initial pour le fondu d'entrée, utilisé pour le fondu d'entrée du son
	int m_fadeOutMs = 0; // Durée du fondu de sortie en millisecondes, utilisée pour le fondu de sortie du son
	float m_soundReverberation = 0.0f; // Facteur de réverbération du son, compris entre 0.0f (pas de réverbération) et 1.0f (réverbération maximale)
	float m_maxReverberation = 2000.0f;

	Vector2f m_relativePosition = Vector2f(0.0f, 0.0f); //Position relative du son par rapport au listener, utilisé pour la spatialisation (calculé à partir de la position du son et du listener)

	// Volume "artistique" propre au fichier (0.0f à 1.0f). 
	// Fixé une fois au chargement pour équilibrer les sons entre eux (ex: baisser un bruit de pas trop fort).
	float m_absoluteVolume = 1.0f;

	// Volume final calculé (m_absoluteVolume * Volume Global du Manager).
	// C'est ce gain brut qui est envoyé à MIX_SetTrackGain AVANT que la SDL3 n'applique son atténuation 3D.
	float m_relativeVolume = 1.0f;

	float m_lastGlobalVolume = 1.0f;

	float m_minDistanceToListen = 0.0f; // Distance minimale à laquelle le son peut être entendu, en pixels
	float m_maxDistanceToListen = 0.0f; // Distance maximale à laquelle le son peut être entendu, en pixels

	// Dans Sound.h
	float m_currentSmoothedGain = 0.0f;
	float m_currentSmoothedPanX = 0.0f;
	float m_currentSmoothedPanZ = 0.0f;

public:

	//Constructeur par défaut de Sound
	Sound(MIX_Mixer* p_mixer, Vector2i p_wSize);
	//Destructeur de Sound
	~Sound();
	//Initialiser le son en fonction du chemin du fichier audio et de sa position dans le monde
	void Initialize(const std::string& p_path, Vector2f p_position);

	void RemoveOldTrack();
	//Fonction statique pour détecter le format de l'audio en fonction de son extension de fichier
	AudioFormat DetectAudioFormat(const std::string& p_path);

	void Play(const Vector2f& p_listenerPosition = Vector2f(0.0f, 0.0f));
	//Arrêter le son
	void Stop();
	//Mettre en pause le son
	void Pause() const;
	//Reprendre la lecture du son
	void Resume() const;

	//Obtenir le format de l'audio
	AudioFormat GetAudioFormat() const { return m_audioFormat; }

	void SetName(const std::string& p_name) { m_name = p_name; }
	std::string GetName() const { return m_name; }

	//SET les properties

	// Définit le nombre de boucles pour la lecture du son (-1 = boucle infinie, 0 = pas de boucle, NB > 0 = nombre de boucles).
	void SetLoopCount(int p_loopCount = 0);
	// Définit le point de départ en secondes pour la lecture du son.
	void SetStartAtTime(int p_startTime = 0);
	// Définit le point de départ de la boucle en secondes pour la lecture du son.
	void SetLoopStartAtTime(int p_startTime = 0);
	// Définit le temps maximum de lecture du son en secondes. Si le son dépasse cette durée, il sera arrêté automatiquement.
	void SetMaxTimeToPlay(int p_maxTime = -1);
	// Définit le volume initial et la durée du fondu d'entrée en millisecondes le volume est un entier entre 0 et 100, où 0 est silencieux et 100 est le volume maximum
	void SetVolumeFadeIn(int p_ms = 0, int p_volume = 0);
	// Définit le volume initial et la durée du fondu de sortie en millisecondes le volume est un entier entre 0 et 100, où 0 est silencieux et 100 est le volume maximum
	void SetVolumeFadeOut(int p_ms = 0);
	// Définit la réverbération du son en fonction d'un facteur de réverbération (p_reverb) compris entre 0.0f (pas de réverbération) et 1.0f (réverbération maximale).
	void SetSoundReverberation(float p_reverb);

	void SetMaxReverberation(float p_maxReverberation) { m_maxReverberation = p_maxReverberation; }

	void SetRelativePosition(const Vector2f& p_relativePosition) { m_relativePosition = p_relativePosition; }
	Vector2f GetRelativePosition() const { return m_relativePosition; }

	int GetVolume() const { return static_cast<int>(m_absoluteVolume * 100.0f); }
	// Définit le volume du son en pourcentage (0 à 100). Le volume est converti en une valeur flottante entre 0.0f et 1.0f pour l'utilisation interne.
	void SetAbsoluteVolume(int p_volume);
	void UpdateRelativeVolume(float p_globalVolume);

	// La fonction SetLimitDistanceToListen définit les distances minimale et maximale à laquelle
	// le son peut être entendu
	// Exemple d'utilisation : SetLimitDistanceToListen(100.0f, 1000.0f);
	void SetLimitDistanceToListen(float p_min, float p_max);

	void ApplySpatialization(const Vector2f& p_listenerPosition, float p_deltaTime);

	float ComputeDistanceGain(float p_distance) const;

	void UpdateRelativePosition(const Vector2f& p_listenerPosition);
	void ComputeSpatialization(MIX_Point3D& p_outPosition, float& p_outGain) const;
};

#include "Sound.inl"
