#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "2DPendentLib/Vector2.h"
#include "2DPendentLib/Sound.h"
#include "2DPendentLib/SoundOrientation.h"
#include "2DPendentLib/AudioFormat.h"

class Entity;
class SoundManager
{
private:

	//Map pour stocker les sons, avec l'identifiant du son (extrait du nom du fichier) comme clé et un pointeur vers l'objet Sound comme valeur
	std::unordered_map<std::string, size_t> m_sounds;

	//Vector pour stocker les sons, utilisé pour itérer plus rapidement lors de la mise à jour des positions des sons et de la gestion de la spatialisation
	std::vector<Sound*> m_soundsVector;

	Entity* m_listener = nullptr; //Pointeur vers l'entité qui est le "listener" pour la spatialisation des sons, généralement le joueur

	bool m_isPausedMusic = false; //Bool pour savoir si les MUSIC sont actuellement en pause, utile pour la fonction PauseAll() et ResumeAll()
	bool m_isPausedSFX = false; //Bool pour savoir si les SFX sont actuellement en pause, utile pour la fonction PauseAll() et ResumeAll()

	//Bool pour savoir si la spatialisation est activée ou non, utile pour la fonction Play() pour décider d'appliquer ou non les paramètres de spatialisation
	bool m_isSpatializationEnabled = true;

	float m_maxAudibleDistance; // Distance maximale à laquelle un son peut être entendu en pixels
	float m_ratioDistanceToVolume = 255.0f / m_maxAudibleDistance; // Ratio pour convertir la distance en distance pour le volume/pixels (0 = proche, 255 = loin) par défault

	//Bool pour savoir si la distance maximale audible doit être calculée automatiquement en fonction de la taille de la fenêtre, ou définie manuellement
	bool m_autoMaxDistance = true;

public:

	//Constructeur par défaut de SoundManager
	SoundManager();
	//Destructeur de SoundManager
	~SoundManager();

	//Singleton pour accéder à SoundManager
	static SoundManager* Get();

	void Update();

	void AddSound(const std::string& p_path);

	//Ajouter un son à la map de SoundManager en utilisant le chemin du fichier audio et sa position dans le monde
	void AddSound(const std::string& p_path, Vector2f p_position);

	void RemoveSound(const std::string& p_id);

	void RemoveAllSounds();

	Sound* GetSound(const std::string& p_id) const;

	//Fonction pour appller la fonction Play() de Sound en utilisant l'identifiant du son,
	// avec une option pour le faire en boucle (-1 = boucle infinie, 0 = pas de boucle, NB > 0 = nombre de boucles)
	// /////////////////// La spatialisation est appliquée uniquement aux SFX (WAV) et non aux MUSIC (MP3) \\\\\\\\\\\\\\\\\\
	// et des paramètres pour la spatialisation (angle et distance)
	//Pour les angles les principale directions sont définies comme suit :
	//NORTH est définie à 0 degrés, NORTHEAST est définie à 45 degrés, EAST est définie à 90 degrés,
	//SOUTHEAST est définie à 135 degrés, SOUTH est définie à 180 degrés, SOUTHWEST est définie à 225 degrés,
	//WEST est définie à 270 degrés, NORTHWEST est définie à 315 degrés
	//distance est la distance entre le son et le joueur, en pourcentage (0 = proche, 255 = loin)
	void Play(const std::string& p_id, int p_loops = 0, SoundOrientation p_angle = SoundOrientation::NORTH, Uint8 p_distance = 0);
	//Fonction pour appeler la fonction Stop() de Sound en utilisant l'identifiant du son
	void Stop(const std::string& p_id);
	//Fonction pour appeler la fonction Pause() de Sound en utilisant l'identifiant du son
	void Pause(const std::string& p_id);
	//Fonction pour appeler la fonction Resume() de Sound en utilisant l'identifiant du son
	void Resume(const std::string& p_id);

	//Fonctions pour appeler les fonctions Pause() de tous les sons dans la map de SoundManager
	void PauseAll();
	//Fonctions pour appeler les fonctions Resume() de tous les sons dans la map de SoundManager
	void ResumeAll();
	//Fonctions pour appeler les fonctions Stop() de tous les sons dans la map de SoundManager
	void StopAll();

	//Fonction pour changer le volume des SFX ou des MUSIC
	//Le volume est un entier entre 0 et 128, où 0 est silencieux et 128 est le volume maximum
	//AudioFormat est utilisé pour spécifier si le volume à changer est celui des SFX (WAV) ou des MUSIC (MP3)
	void SetVolume(int p_volume, AudioFormat p_format);
	//Fonction pour augmenter le volume des SFX ou des MUSIC d'une certaine quantité
	void IncreaseVolume(int p_amount, AudioFormat p_format);
	//Fonction pour diminuer le volume des SFX ou des MUSIC d'une certaine quantité
	void DecreaseVolume(int p_amount, AudioFormat p_format);

	//Fonction pour obtenir le volume actuel des SFX
	int GetSFXVolume() const { return Mix_Volume(-1, -1); }
	//Fonction pour obtenir le volume actuel des MUSIC
	int GetMusicVolume() const { return Mix_VolumeMusic(-1); }

#ifdef _DEBUG
	//Fonction pour afficher les volumes actuels des SFX et des MUSIC dans la console pour le debug
	void DebugVolume() const;
	//Fonction pour afficher la position du listener dans la console pour le debug
	void DebugListenerPosition() const;
#endif

	//Fonction pour mettre à jour la position d'un son spécifique dans la map de SoundManager en utilisant la fonction UpdatePosition() de Sound,
	// en fonction de la position du joueur pour la spatialisation
	void SetSoundPosition(const std::string& p_id, Vector2f p_pos);
	//Fonction pour obtenir la position d'un son spécifique dans la map de SoundManager
	Vector2f GetSoundPosition(const std::string& p_id) const;

	//Fonction template pour obtenir la distance entre un son et une entité spécifique, en utilisant la position de l'entité et du son pour calculer la distance euclidienne
	template<typename T>
	float GetDistanceTo(Sound* p_sound, T* p_entity);

	//Fonction template pour définir une entité spécifique comme listener pour la spatialisation des sons, en stockant un pointeur vers l'entité dans SoundManager
	template<typename T>
	void SetListener(T* p_entity);

	//Fonction pour obtenir le pointeur vers l'entité qui est le listener pour la spatialisation des sons
	Entity* GetListener() const { return m_listener; }

	//Fonction pour mettre en pause ou reprendre un son spécifique en utilisant son identifiant
	void SetPause_Resume_Sound(const std::string& p_id);

	//Fonction pour activer ou désactiver la spatialisation des sons, en inversant la valeur du bool m_isSpatializationEnabled
	void SetSpatializationEnabled_Disable();
	//Fonction pour obtenir le bool pour savoir si la spatialisation des sons est activée ou non
	bool IsSpatializationEnabled() const { return m_isSpatializationEnabled; }

	//Fonction pour appliquer la spatialisation à un son spécifique en utilisant la position du son et du listener pour calculer l'angle et la distance,
	void ApplySpatialization(Sound* p_sound);

	void SetMaxAudibleDistance(float p_distance);
	float GetMaxAudibleDistance() const { return m_maxAudibleDistance; }

	void SetRatioDistanceToVolume(float p_ratio);
	float GetRatioDistanceToVolume() const { return m_ratioDistanceToVolume; }

	void SetAutoMaxDistance(bool p_auto) { m_autoMaxDistance = p_auto; }
	bool GetAutoMaxDistance() const { return m_autoMaxDistance; }
};

#include "SoundManager.inl"