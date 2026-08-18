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
	//std::map pour stocker les scènes, indexées par un identifiant unique (string)
	std::unordered_map<std::string, size_t> m_scenes;
	//vector pour stocker les pointeurs vers les scènes, utilisé pour itérer plus rapidement lors de la mise à jour et de la destruction des scènes
	std::vector<Scene*> m_scenesVector;

	//Pointeur vers la scène actuellement active
	//Scene* m_currentScene = nullptr;
	std::unordered_map<std::string, size_t> m_scenesInStack;
	std::vector<Scene*> m_scenesInStackVector; //Vector pour stocker les scènes actuellement update, utilisé pour itérer plus rapidement lors de la mise à jour des scènes

	std::vector<Entity*> m_entToDraw;

	//void ChangeCurrentScene();

	//std::string m_tempCurrentSceneID;

public:

	friend class Application;
	
	//Constructeur par défaut de SceneManager
	SceneManager();
	//Destructeur de SceneManager, il est responsable de la destruction de toutes les scènes stockées dans m_scenes pour éviter les fuites de mémoire
	~SceneManager();

	//Singleton du SceneManager
	static SceneManager* Get();

	//Fonction template pour ajouter une scène de type T à m_scenes, où T doit être une classe qui hérite de Scene
	template<typename T>
	void AddScene(const std::string& p_id);

	void SetPersistentScene(Scene* p_scene) { m_persistentScene = p_scene; }

	//Fonction pour supprimer une scène de m_scenes en utilisant son identifiant unique (string)
	void RemoveScene(const std::string& p_id);
	//Fonction pour supprimer toutes les scènes de m_scenes
	void ClearScenes();

	//Fonction pour mettre à jour la scène actuellement active
	//void UpdateCurrentScene();

	//Fonction pour définir la scène actuellement active en utilisant son identifiant unique (string)
	//void SetCurrentScene(const std::string& p_id);

	//Fonction pour obtenir un pointeur vers la scène actuellement active
	//Scene* GetCurrentScene() const { return m_currentScene; }

	void PushScene(const std::string& p_id);

	void PopScene(const std::string& p_id);

	void ClearScenesInStack();

	void EventScenesInStack();

	void UpdateScenesInStack();

	void DrawScenesInStack(Render* f_render, Window* f_window, Camera* p_camera);

	bool IsSceneInStack() const;

	Scene* GetScene(const std::string& p_id) const;

	template<typename T>
	T* GetScene(const std::string& p_id) const;

};

#include "SceneManager.inl"
