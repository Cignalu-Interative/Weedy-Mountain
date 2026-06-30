#pragma once
#include <string>
#include <list>
#include <vector>
#include "2DPendentLib/Vector2.h"
#include "2DPendentLib/SoundOrientation.h"
#include "2DPendentLib/Color.h"
#include "2DPendentLib/Drawable.h"


class SoundManager;
enum class Shape : uint8_t;
class Entity;
class Application;
class AssetManager;
class Camera;
class Geometry;
class Button;
class Slider;

class Scene : public Drawable
{
private:

	//Pointeur vers l'application
	Application* m_application = nullptr;

	//setter pour le pointeur vers l'application
	void SetApplication(Application* f_application) { m_application = f_application; }

	std::string m_name; //Nom de la scène, utilisé pour l'identifier dans le SceneManager

	bool m_isOnStateSleep = false; //Indique si la scène est dans l'état de veille est donc si OnSleep() a été appelé
	bool m_isOnStateWake = false; //Indique si la scène est dans l'état de réveil est donc si OnWake() a été appelé

	bool m_isVisible = true; //Indique si la scène est visible ou non, si elle n'est pas visible, les entités de la scène ne seront pas dessinées à l'écran

	bool m_isSleepEvent = false; //Indique si la scène est en veille ou non, si elle est en veille, la fonction OnEvent() ne sera pas appelée
	bool m_isSleepUpdate = false; //Indique si la scène est en veille ou non, si elle est en veille, la fonction OnUpdate() ne sera pas appelée
	bool m_isSleepDraw = false; //Indique si la scène est en veille ou non, si elle est en veille, la fonction OnDraw() ne sera pas appelée

	std::list<Entity*> m_entities; //Vector de pointeurs vers les entités présentes dans la scène
	std::list<Geometry*> m_geometries; //Vector de pointeurs vers les géometries présentes dans la scène, utilisé pour les éléments de décor et les éléments de l'interface utilisateur

	// TO DO faire la meme chose pour les map tiled

	//Indique si la scène est dans le cache du SceneManager ou non, si elle est dans le cache,
	//elle ne sera pas détruite lorsqu'elle sera retirée de la scène active, mais simplement mise en veille
	bool m_isInCache = false;

	void Initialize();
	void Event();
	void Update();
	void Sleep();
	void Wake();
	void Destroy();
	void Draw(Render* f_render, Window* f_window, Camera* p_camera) override;

protected:

	//Constructeur par défaut de Scene
	Scene() = default;
	//Destructeur de Scene, il est virtuel pour permettre aux classes qui héritent de Scene de définir leur propre logique de destruction
	virtual ~Scene() {};

	void SetName(const std::string& p_name) { m_name = p_name; }
	std::string GetName() const { return m_name; }

	//Fonction virtuelle pure pour permettre aux classes qui héritent de Scene de définir leur propre logique d'initialisation
	virtual void OnInitialize() {};
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
	//Fonction virtuelle pure pour permettre aux classes qui héritent de Scene de définir leur propre logique de dessin
	virtual void OnDraw() = 0;

public:

	//Fonction template pour créer une entité de type T
	template<typename T>
	T* CreateEntity(Vector2f p_position, Vector2i p_dimension, const std::string& p_path, Shape p_shape, bool p_isGUI = false, bool p_animation = false, const std::string& p_jsonAnimation = "");

	template<typename T>
	T* CreateButton(Vector2f p_position, Vector2i p_dimention, const std::string& p_spritePath, const std::string& p_textID, const std::string& p_text, bool p_isGUI = true);
	Button* CreateButton(Vector2f p_position, Vector2i p_dimention, const std::string& p_spritePath, const std::string& p_textID, const std::string& p_text, bool p_isGUI = true);

	Slider* CreateSlider(Vector2f p_position, Vector2i p_dimention, const std::string& p_spritePath, const std::string& p_id, int p_stateCount = -1, bool p_isGUI = true);

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

	// Setter pour le bool m_visible, si m_visible est false, les entités de la scène ne seront pas dessinées à l'écran
	void SetIsVisible(bool p_visible);
	// Getter pour le bool m_visible
	bool GetIsVisible() const { return m_isVisible; }

	void AddEntity(Entity* p_entity);

	void RemoveEntity(Entity* p_entity);

	std::list<Entity*>& GetEntities() { return m_entities; }

	void RemoveGeometry(const std::string& p_id);

	template<typename T>
	T* GetGeometry(const std::string& p_id);

	std::list<Geometry*>& GetGeometries() { return m_geometries; }

	void SetIsOnStateSleep(bool p_isOnStateSleep) { m_isOnStateSleep = p_isOnStateSleep; }
	bool GetIsOnStateSleep() const { return m_isOnStateSleep; }

	void SetIsOnStateWake(bool p_isOnStateWake) { m_isOnStateWake = p_isOnStateWake; }
	bool GetIsOnStateWake() const { return m_isOnStateWake; }

	// Setter pour le bool m_sleepEvent, si m_sleepEvent est true, la fonction OnEvent() ne sera pas appelée
	// pour freeze placer au debut un if = true alors return
	void SetSleepEvent(bool p_sleep) { m_isSleepEvent = p_sleep; }
	bool GetSleepEvent() const { return m_isSleepEvent; }

	// Setter pour le bool m_sleepUpdate, si m_sleepUpdate est true, la fonction OnUpdate() ne sera pas appelée
	// pour freeze placer au debut un if = true alors return
	void SetSleepUpdate(bool p_sleep) { m_isSleepUpdate = p_sleep; }
	bool GetSleepUpdate() const { return m_isSleepUpdate; }

	void SetSleepDraw(bool p_sleep) { m_isSleepDraw = p_sleep; }
	bool GetSleepDraw() const { return m_isSleepDraw; }

	void SetIsInCache(bool p_isInCache) { m_isInCache = p_isInCache; }
	bool GetIsInCache() const { return m_isInCache; }

	//DEBUG
	void CreatePoint(const std::string& p_id, Vector2f p_position, Color p_color, bool p_isGUI = false, bool p_clampToScreen = true);
	// Fonction pour créer une ligne à dessiner
	void CreateLine(const std::string& p_id, Vector2f p_start, Vector2f p_end, Color p_color, bool p_isGUI = false, bool p_clampToScreen = true);
	// Fonction pour créer un rectangle à dessiner
	void CreateRect(const std::string& p_id, Vector2f p_position, Vector2i p_dimension, Color p_color, bool p_isGUI = false, bool p_clampToScreen = true);
	// Fonction pour créer un rectangle rempli à dessiner
	void CreateFillRect(const std::string& p_id, Vector2f p_position, Vector2i p_dimension, Color p_color, bool p_isGUI = false, bool p_clampToScreen = true);
	// Fonction pour créer un cercle à dessiner
	void CreateCircle(const std::string& p_id, Vector2f p_position, Vector2i p_dimension, Color p_color, bool p_isGUI = false, bool p_clampToScreen = true);
	// Fonction pour créer un cercle rempli à dessiner
	void CreateFillCircle(const std::string& p_id, Vector2f p_position, Vector2i p_dimension, Color p_color, bool p_isGUI = false, bool p_clampToScreen = true);
	// Fonction pour créer un texte à dessiner
	//faire un create qui renvoie une addresse de text pour l'update
	void CreateText(const std::string& p_id, Vector2f p_position, const std::string& p_text, Color p_color, bool p_isGUI = false, bool p_clampToScreen = true);

	Entity* GetEntityByTag(int p_tag);
	std::vector<Entity*> GetEntitiesByTag(int p_tag);

	void DestroyEntityByTag(int p_tag);
	void DestroyEntitiesByTag(int p_tag);

	//Liste des class amie avec la class Scene
	friend class Application;
	friend class SceneManager;
	friend class Entity;
};

#include "Scene.inl"
