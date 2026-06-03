#pragma once
#include <string>
#include "2DPendentLib/Vector2.h"
#include "2DPendentLib/SoundOrientation.h"

class SoundManager;
enum class Shape : uint8_t;
class Entity;
class Application;
class AssetManager;
class Camera;
class Scene
{
private:

	//Pointeur vers l'application
	Application* m_application = nullptr;

	//setter pour le pointeur vers l'application
	void SetApplication(Application* f_application) { m_application = f_application; }

	std::string m_name; //Nom de la scène, utilisé pour l'identifier dans le SceneManager

	bool m_sleepUpdate = false; //Indique si la scène est en veille ou non, si elle est en veille, la fonction OnUpdate() ne sera pas appelée

	bool m_isVisible = true; //Indique si la scène est visible ou non, si elle n'est pas visible, les entités de la scène ne seront pas dessinées à l'écran

	bool m_isSleepEvent = false; //Indique si la scène est en veille ou non, si elle est en veille, la fonction OnEvent() ne sera pas appelée
	bool m_isSleepUpdate = false; //Indique si la scène est en veille ou non, si elle est en veille, la fonction OnUpdate() ne sera pas appelée

	std::vector<Entity*> m_entities; //Vector de pointeurs vers les entités présentes dans la scène

	// TO DO faire la meme chose pour les map tiled

protected:

	//Constructeur par défaut de Scene
	Scene() = default;
	//Destructeur de Scene, il est virtuel pour permettre aux classes qui héritent de Scene de définir leur propre logique de destruction
	virtual ~Scene() {};

	void SetName(const std::string& p_name) { m_name = p_name; }
	std::string GetName() const { return m_name; }

	//Fonction virtuelle pure pour permettre aux classes qui héritent de Scene de définir leur propre logique d'initialisation
	virtual void OnInitialize() = 0; 
	//Fonction virtuelle pure pour permettre aux classes qui héritent de Scene de définir leur propre logique de gestion des événements
	virtual void OnEvent() = 0; 
	//Fonction virtuelle pure pour permettre aux classes qui héritent de Scene de définir leur propre logique de mise à jour
	virtual void OnUpdate() = 0; 
	//Fonction virtuelle pure pour permettre aux classes qui héritent de Scene de définir leur propre logique de mise en veille
	virtual void OnSleep() = 0;
	//Fonction virtuelle pure pour permettre aux classes qui héritent de Scene de définir leur propre logique de réveil
	virtual void OnWake() = 0;
	//Fonction virtuelle pure pour permettre aux classes qui héritent de Scene de définir leur propre logique de destruction
	virtual void OnDestroy() = 0; 

	//Fonction template pour créer une entité de type T
	template<typename T>
	T* CreateEntity(Vector2f p_position, Vector2i p_dimension, const std::string& p_path, Shape p_shape, bool p_isGUI = false);

public:

	//Fonction pour obtenir la largeur de la fenêtre de jeu en utilisant la fonction GetWindowSize() de l'application
	int GetWindowWidth() const; 
	//Fonction pour obtenir la hauteur de la fenêtre de jeu en utilisant la fonction GetWindowSize() de l'application
	int GetWindowHeight() const; 
	//Fonction pour obtenir le temps écoulé entre deux frames en nanosecondes
	float GetDeltaTime() const; 

	//Fonction pour changer la valeur de m_isRunning de l'application en utilisant la fonction m_isRunning de l'application
	void ChangeisRunning(bool p_bool); 
	//Fonction pour changer la valeur de m_autoFPS de l'application en utilisant la fonction m_autoFPS de l'application
	void ChangeAutoFPS(); 
	//Fonction pour obtenir la valeur de m_autoFPS de l'application en utilisant la fonction m_autoFPS de l'application
	bool GetAutoFPS(); 
	//Fonction pour définir la valeur de m_FPS de l'application en utilisant la fonction m_FPS de l'application
	void SetFPS(int p_newFPS); 
	//Fonction pour obtenir la valeur de m_FPS de l'application en utilisant la fonction m_FPS de l'application
	int GetCurrentFPS() const;

	//Fonction pour obtenir un pointeur vers l'application
	Application* GetApplication() const;

	Camera* GetCamera() const;

	// Setter pour le bool m_sleepUpdate, si m_sleepUpdate est true, la fonction OnUpdate() ne sera pas appelée
	void SetSleepUpdate(bool p_sleep) { m_sleepUpdate = p_sleep; }
	// Getter pour le bool m_sleepUpdate
	bool GetSleepUpdate() const { return m_sleepUpdate; }

	// Setter pour le bool m_visible, si m_visible est false, les entités de la scène ne seront pas dessinées à l'écran
	void SetIsVisible(bool p_visible);
	// Getter pour le bool m_visible
	bool GetIsVisible() const { return m_isVisible; }

	void AddEntity(Entity* p_entity);

	void RemoveEntity(Entity* p_entity);

	std::vector<Entity*>& GetEntities() { return m_entities; }

	// Setter pour le bool m_sleepEvent, si m_sleepEvent est true, la fonction OnEvent() ne sera pas appelée
	// pour freeze placer au debut un if = true alors return
	void SetSleepEvent(bool p_sleep) { m_isSleepEvent = p_sleep; }
	bool GetSleepEvent() const { return m_isSleepEvent; }

	// Setter pour le bool m_sleepUpdate, si m_sleepUpdate est true, la fonction OnUpdate() ne sera pas appelée
	// pour freeze placer au debut un if = true alors return
	void SetSleepUpdate(bool p_sleep) { m_isSleepUpdate = p_sleep; }
	bool GetSleepUpdate() const { return m_isSleepUpdate; }

	//Liste des class amie avec la class Scene
	friend class Application;
	friend class SceneManager;
	friend class Entity;
};

#include "Scene.inl"