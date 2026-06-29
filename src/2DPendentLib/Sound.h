#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <map>
#include "Vector2.h"
#include "AudioFormat.h"
#include "Transformable.h"

class Sound : public Transformable
{
private:

	std::string m_name; //Nom du son, extrait du nom du fichier audio, utilisé comme identifiant dans SoundManager

	Mix_Chunk* m_WAV = nullptr; //Pointeur vers un chunk de son pour les fichiers WAV
	Mix_Music* m_MUSIC = nullptr; //Pointeur vers un chunk de musique pour les fichiers musicaux MP3

	int m_lastChannel = -1; //Pour stocker le dernier canal utilisé pour jouer un son, utile pour la spatialisation

	//Format de l'audio par default à NONE, il sera défini lors de l'initialisation en fonction du format du fichier audio
	AudioFormat m_audioFormat = AudioFormat::NONE;
	
	float m_distanceSquared = 0.0f; //Distance au carré entre le son et le listener, utilisé pour la spatialisation (calculé à partir de la position du son et du listener)

public:

	//Constructeur par défaut de Sound
	Sound() = default;
	//Destructeur de Sound
	~Sound();
	//Initialiser le son en fonction du chemin du fichier audio et de sa position dans le monde
	void Initialize(const std::string& p_path, Vector2f p_position);
	//Fonction statique pour détecter le format de l'audio en fonction de son extension de fichier
	AudioFormat DetectAudioFormat(const std::string& p_path);

	//Jouer le son, avec une option pour le faire en boucle (-1 = boucle infinie, 0 = pas de boucle, NB > 0 = nombre de boucles)
	// /////////////////// La spatialisation est appliquée uniquement aux SFX (WAV) et non aux MUSIC (MP3) \\\\\\\\\\\\\\\\\\
	// et des paramètres pour la spatialisation (angle et distance)
	//Pour les angles les principale directions sont définies comme suit :
	//NORTH est définie à 0 degrés, NORTHEAST est définie à 45 degrés, EAST est définie à 90 degrés,
	//SOUTHEAST est définie à 135 degrés, SOUTH est définie à 180 degrés, SOUTHWEST est définie à 225 degrés,
	//WEST est définie à 270 degrés, NORTHWEST est définie à 315 degrés
	//distance est la distance entre le son et le joueur, en pourcentage (0 = proche, 255 = loin)
	void Play(int p_loops = 0, Sint16 p_angle = 0, Uint8 p_distance = 0);
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

	void SetDistanceSquared(float p_distanceSquared) { m_distanceSquared = p_distanceSquared; }
	float GetDistanceSquared() const { return m_distanceSquared; }

	int GetChannel() const { return m_lastChannel; }
};

#include "Sound.inl"
