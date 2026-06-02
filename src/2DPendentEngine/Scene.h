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
class Button;
class Slider;

class Scene
{
private:

	//Pointeur vers l'application
	Application* m_application = nullptr;

	//setter pour le pointeur vers l'application
	void SetApplication(Application* f_application) { m_application = f_application; }

	std::string m_name; //Nom de la scène, utilisé pour l'identifier dans le SceneManager

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
	//Fonction virtuelle pure pour permettre aux classes qui héritent de Scene de définir leur propre logique de destruction
	virtual void OnDestroy() = 0; 

	//Fonction template pour créer une entité de type T
	template<typename T>
	T* CreateEntity(Vector2f p_position, Vector2i p_dimension, const std::string& p_path, Shape p_shape, bool p_isGUI = false);

	Button* CreateButton(Vector2f p_position, Vector2i p_dimention, const std::string& p_spritePath, const std::string& p_textID, const std::string& p_text, bool p_isGUI = true);

	Slider* CreateSlider(Vector2f p_position, Vector2i p_dimention, const std::string& p_spritePath, int p_stateCount = -1, bool p_isGUI = true);

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

	int GetCurrentFPS() const;

	//Fonction pour obtenir un pointeur vers l'application
	Application* GetApplication() const;

	Camera* GetCamera() const;

	//Liste des class amie avec la class Scene
	friend class Application;
	friend class SceneManager;
	friend class Entity;
};

#include "Scene.inl"