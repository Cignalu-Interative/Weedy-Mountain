#pragma once
#include <unordered_map>
#include <vector>
#include <string>

class Scene;
class Application;
class Render;
class Window;
class Camera;
class Entity;

class SceneManager
{
private:
	Scene* m_persistentScene = nullptr;
	//std::map pour stocker les sc�nes, index�es par un identifiant unique (string)
	std::unordered_map<std::string, size_t> m_scenes;
	//vector pour stocker les pointeurs vers les sc�nes, utilis� pour it�rer plus rapidement lors de la mise � jour et de la destruction des sc�nes
	std::vector<Scene*> m_scenesVector;

	//Pointeur vers la sc�ne actuellement active
	//Scene* m_currentScene = nullptr;
	std::unordered_map<std::string, size_t> m_scenesInStack;
	std::vector<Scene*> m_scenesInStackVector; //Vector pour stocker les sc�nes actuellement update, utilis� pour it�rer plus rapidement lors de la mise � jour des sc�nes

	std::vector<Entity*> m_entToDraw;

	//void ChangeCurrentScene();

	//std::string m_tempCurrentSceneID;

public:

	friend class Application;
	
	//Constructeur par d�faut de SceneManager
	SceneManager();
	//Destructeur de SceneManager, il est responsable de la destruction de toutes les sc�nes stock�es dans m_scenes pour �viter les fuites de m�moire
	~SceneManager();

	//Singleton du SceneManager
	static SceneManager* Get();

	//Fonction template pour ajouter une sc�ne de type T � m_scenes, o� T doit �tre une classe qui h�rite de Scene
	template<typename T>
	void AddScene(const std::string& p_id);

	void SetPersistentScene(Scene* p_scene) { m_persistentScene = p_scene; }

	//Fonction pour supprimer une sc�ne de m_scenes en utilisant son identifiant unique (string)
	void RemoveScene(const std::string& p_id);
	//Fonction pour supprimer toutes les sc�nes de m_scenes
	void ClearScenes();

	//Fonction pour mettre � jour la sc�ne actuellement active
	//void UpdateCurrentScene();

	//Fonction pour d�finir la sc�ne actuellement active en utilisant son identifiant unique (string)
	//void SetCurrentScene(const std::string& p_id);

	//Fonction pour obtenir un pointeur vers la sc�ne actuellement active
	//Scene* GetCurrentScene() const { return m_currentScene; }

	void PushScene(const std::string& p_id);

	void PopScene(const std::string& p_id);

	void ClearScenesInStack();

	void EventScenesInStack();

	void UpdateScenesInStack();

	void DrawScenesInStack(Render* f_render, Window* f_window, Camera* p_camera);

	bool IsSceneInStack() const;

	Scene* GetScene(const std::string& p_id) const;

};

#include "SceneManager.inl"
