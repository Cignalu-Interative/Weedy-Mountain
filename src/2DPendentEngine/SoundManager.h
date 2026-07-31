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

	MIX_Mixer* m_mixer = nullptr; //Pointeur vers le mixer, utilis� pour g�rer les sons et la musique

	//Map pour stocker les sons, avec l'identifiant du son (extrait du nom du fichier) comme cl� et un pointeur vers l'objet Sound comme valeur
	std::unordered_map<std::string, size_t> m_sounds;

	//Vector pour stocker les sons, utilis� pour it�rer plus rapidement lors de la mise � jour des positions des sons et de la gestion de la spatialisation
	std::vector<Sound*> m_soundsVector;

	Entity* m_listener = nullptr; //Pointeur vers l'entit� qui est le "listener" pour la spatialisation des sons, g�n�ralement le joueur

	bool m_isPausedMusic = false; //Bool pour savoir si les MUSIC sont actuellement en pause, utile pour la fonction PauseAll() et ResumeAll()
	bool m_isPausedSFX = false; //Bool pour savoir si les SFX sont actuellement en pause, utile pour la fonction PauseAll() et ResumeAll()

	//Bool pour savoir si la spatialisation est activ�e ou non, utile pour la fonction Play() pour d�cider d'appliquer ou non les param�tres de spatialisation
	bool m_isSpatializationEnabled = true;

	float m_MusiqueVolume = 1.0f; //Volume global pour les MUSIC, compris entre 0.0f (silencieux) et 1.0f (volume maximum)
	float m_SFXVolume = 1.0f; //Volume global pour les SFX, compris entre 0.0f (silencieux) et 1.0f (volume maximum)

public:

	//Constructeur par d�faut de SoundManager
	SoundManager();
	//Destructeur de SoundManager
	~SoundManager();

	//Singleton pour acc�der � SoundManager
	static SoundManager* Get();

	void SetMixer(MIX_Mixer* p_mixer) { m_mixer = p_mixer; }

	void Update(float p_deltatime);

	void AddSound(const std::string& p_path);

	//Ajouter un son � la map de SoundManager en utilisant le chemin du fichier audio et sa position dans le monde
	void AddSound(const std::string& p_path, Vector2f p_position);

	void RemoveSound(const std::string& p_id);

	void RemoveAllSounds();

	Sound* GetSound(const std::string& p_id) const;

	//Fonction pour appller la fonction Play() de Sound en utilisant l'identifiant du son,
	// avec une option pour le faire en boucle (-1 = boucle infinie, 0 = pas de boucle, NB > 0 = nombre de boucles)
	// /////////////////// La spatialisation est appliqu�e uniquement aux SFX (WAV) et non aux MUSIC (MP3) \\\\\\\\\\\\\\\\\\
	// et des param�tres pour la spatialisation (angle et distance)
	//Pour les angles les principale directions sont d�finies comme suit :
	//NORTH est d�finie � 0 degr�s, NORTHEAST est d�finie � 45 degr�s, EAST est d�finie � 90 degr�s,
	//SOUTHEAST est d�finie � 135 degr�s, SOUTH est d�finie � 180 degr�s, SOUTHWEST est d�finie � 225 degr�s,
	//WEST est d�finie � 270 degr�s, NORTHWEST est d�finie � 315 degr�s
	//distance est la distance entre le son et le joueur, en pourcentage (0 = proche, 255 = loin)
	void Play(const std::string& p_id);
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

	//Fonctions pour d�finir le volume global des MUSIC, compris entre 0 (silencieux) et 100 (volume maximum)
	void SetMusicVolume(int p_volume);
	//Fonctions pour d�finir le volume global des SFX, compris entre 0 (silencieux) et 100 (volume maximum)
	void SetSFXVolume(int p_volume);

	//Fonctions pour augmenter le volume global des MUSIC, compris entre 0 (silencieux) et 100 (volume maximum)
	void IncreaseMusicVolume(int p_amount);
	//Fonctions pour augmenter le volume global des SFX, compris entre 0 (silencieux) et 100 (volume maximum)
	void IncreaseSFXVolume(int p_amount);

	//Fonctions pour diminuer le volume global des MUSIC, compris entre 0 (silencieux) et 100 (volume maximum)
	void DecreaseMusicVolume(int p_amount);
	//Fonctions pour diminuer le volume global des SFX, compris entre 0 (silencieux) et 100 (volume maximum)
	void DecreaseSFXVolume(int p_amount);

	//Fonction pour obtenir le volume actuel des SFX
	int GetSFXVolume() const;
	//Fonction pour obtenir le volume actuel des MUSIC
	int GetMusicVolume() const;

#ifdef _DEBUG
	//Fonction pour afficher les volumes actuels des SFX et des MUSIC dans la console pour le debug
	void DebugVolume() const;
	//Fonction pour afficher la position du listener dans la console pour le debug
	void DebugListenerPosition() const;
#endif

	//Fonction pour mettre � jour la position d'un son sp�cifique dans la map de SoundManager en utilisant la fonction UpdatePosition() de Sound,
	// en fonction de la position du joueur pour la spatialisation
	void SetSoundPosition(const std::string& p_id, Vector2f p_pos);
	//Fonction pour obtenir la position d'un son sp�cifique dans la map de SoundManager
	Vector2f GetSoundPosition(const std::string& p_id) const;

	//Fonction template pour obtenir la distance entre un son et une entit� sp�cifique, en utilisant la position de l'entit� et du son pour calculer la distance euclidienne
	template<typename T>
	float GetDistanceTo(Sound* p_sound, T* p_entity);

	//Fonction template pour d�finir une entit� sp�cifique comme listener pour la spatialisation des sons, en stockant un pointeur vers l'entit� dans SoundManager
	template<typename T>
	void SetListener(T* p_entity);

	//Fonction pour obtenir le pointeur vers l'entit� qui est le listener pour la spatialisation des sons
	Entity* GetListener() const { return m_listener; }

	//Fonction pour mettre en pause ou reprendre un son sp�cifique en utilisant son identifiant
	void SetPause_Resume_Sound(const std::string& p_id);

	//Fonction pour activer ou d�sactiver la spatialisation des sons, en inversant la valeur du bool m_isSpatializationEnabled
	void SetSpatializationEnabled_Disable();
	//Fonction pour obtenir le bool pour savoir si la spatialisation des sons est activ�e ou non
	bool IsSpatializationEnabled() const { return m_isSpatializationEnabled; }

	//Fonction pour appliquer la spatialisation � un son sp�cifique en utilisant la position du son et du listener pour calculer l'angle et la distance,
	void ApplySpatialization(Sound* p_sound, float p_deltaTime);
};

#include "SoundManager.inl"